#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class CLIException : std::exception {
    private:
        std::string error_message;

    public: 
        CLIException(const std::string& message)
        {
            error_message = "[Error] " + message; 
        }

        CLIException(const std::string& function_name, const std::string& message)
        {
            error_message = "[Error]";
            if(!function_name.empty()) {
                error_message.append("[" + function_name + "]");
            }
            error_message.push_back(' ');
            error_message.append(message);
        }

        const char* what() const noexcept override
        {
            return error_message.c_str();
        }
};

class CLI {
    private:
        std::vector<std::string> args;
        std::unordered_map<std::string, std::unordered_map<std::string, int>> subcommands; // valid subcommands and their valid flags
        std::string active_subcommand; // the current active subcommand
        int active_subcommand_end_pos; // stores the position the active subcommand ends in the arguement list
        int max_subcommand_chain_count; // keeps track of how many subcommand words can be chained

        void setActiveSubcommand(int start = 1)
        {
            if(start < 0 || start >= args.size()) {
                return;
            }

            active_subcommand.clear();
            active_subcommand_end_pos = 0;
            if(subcommands.size() < 2) {
                return;
            }

            int chain_end_pos = start + max_subcommand_chain_count - 1;
            if(chain_end_pos >= args.size()) {
                chain_end_pos = args.size()-1;
            }
            std::string temp;
            for(int i = start; i <= chain_end_pos; i++) {
                temp.append(args[i]);   
                if(isValidSubcommand(temp)) {
                    active_subcommand = temp;
                    active_subcommand_end_pos = i;
                }
                temp.push_back(' ');
            }
        }

        void setSubcommands(const std::vector<std::string>& valid_subs, bool clear)
        {
            if(clear) {
                clearSubcommands();
            }
            
            for(int i = 0; i < valid_subs.size(); i++) {
                std::string temp = trim(valid_subs[i]);

                if(temp.empty() || isValidSubcommand(temp)) {
                    continue;
                }

                int max_word_count = 1;
                for(int j = 0; j < temp.size(); j++) {
                    if(temp[j] == ' ') {
                        max_word_count++;
                    }
                }

                if(max_word_count > max_subcommand_chain_count) {
                    max_subcommand_chain_count = max_word_count;
                }

                subcommands.insert({temp, std::unordered_map<std::string, int>()});
            }
        }

        void setFlags(const std::string& subcmd, const std::vector<std::string>& valid_flags, bool clear)
        {
            if(!isValidSubcommand(subcmd)) {
                throw CLIException(__func__, "Invalid subcommand \"" + subcmd + "\"");
            }
            
            if(clear) {
                subcommands.at(subcmd).clear(); // clear the old flags of the given subcommand
            }

            for(int i = 0; i < valid_flags.size(); i++) {
                std::string flag = trim(valid_flags[i]);
                std::string prefix = getFlagPrefix(flag);

                if(prefix.empty() || prefix.size() > 2) {
                    subcommands.at(subcmd).clear();
                    throw CLIException(__func__, "\"" + flag + "\" does not have a proper prefix");
                } else if(prefix.size() == 1 && flag.size() > 2) {
                    subcommands.at(subcmd).clear();
                    throw CLIException(__func__, "\"" + flag + "\" has prefix for a single character format");
                } else if(prefix.size() == flag.size()) {
                    subcommands.at(subcmd).clear();
                    throw CLIException(__func__, "\"" + flag + "\" is not a proper flag");
                }

                subcommands.at(subcmd).insert({flag, -1});
            }
        }

        void initFlags(const std::string& subcmd, int start = -1) // initialize values of flags from args
        {
            if(subcommands.at(active_subcommand).empty()) {
                return;
            }

            if(start < 0) {
                start = active_subcommand_end_pos + 1;
            }

            for(int i = start; i < args.size(); i++) { 
                std::vector<std::string> flags = splitFlags(trim(args[i]));

                if(flags.empty()) {
                    continue;
                } 

                for(int j = 0; j < flags.size(); j++) { // something wrong here
                    if(isValidFlag(flags[j])) {
                        subcommands.at(subcmd)[flags[j]] = i;
                    } else {
                        throw CLIException(__func__, "\"" + flags[j] + "\" is not a valid flag of the \"" + active_subcommand + "\" subcommand");
                    }
                }
            }
        }

        void resetFlags(const std::string& subcmd) // uninitializes all flags of a subcommand
        {
            if(!isValidSubcommand(subcmd) || subcommands.at(subcmd).empty()) {
                return;
            }
            for(auto& i : subcommands.at(subcmd)) {
                i.second = -1;
            }
        }

        std::string trim(const std::string& str) const // removes trailing whitespace
        {
            std::string trimmed;
            trimmed.reserve(str.size());
            int i = 0;
            while(i < str.size() && str[i] == ' ') {
                i++;
            }
            while(i < str.size()) {
                if(str[i] == ' ') {
                    while(i < str.size() && str[i] == ' ') {
                        i++;
                    }
                    if(i >= str.size()) {
                        break;
                    } else {
                        trimmed.push_back(' ');
                    }
                } else if(str[i] == '=') { // turns flags such as "--flag=value" to "--flag"
                    return trimmed;
                }
                trimmed.push_back(str[i]);
                i++;
            }
            return trimmed;
        }

        // checks if a character is flag prefix
        bool isFlagPrefix(char ch) const
        {
            return ch == '-';
        }
        
        // returns the flag prefix of a given string
        std::string getFlagPrefix(const std::string& flag) const
        {
            std::string result;
            for(int i = 0; isFlagPrefix(flag[i]); i++) {
                result.push_back(flag[i]);
            }
            return result;
        } 

        // checks if a given string has a flag prefix
        bool hasFlagPrefix(const std::string& flag) const
        {
            return getFlagPrefix(flag).size() > 0;
        }

        // parses flags like "-hiv" to {"-h", "-i", "-v"}
        std::vector<std::string> splitFlags(const std::string& flag) const
        {
            std::vector<std::string> result;
            std::string temp = getFlagPrefix(flag);

            if(temp.empty() || temp.size() == flag.size()) {
                return result;
            } 
            
            if(temp == "--") {
                result.push_back(flag);
                return result;
            }

            int i = temp.size();
            while(i < flag.size()) {
                temp.push_back(flag[i]);
                result.push_back(temp);
                temp.pop_back();
                i++;
            }

            return result;
        }

    public:
        CLI() : args(), subcommands(), active_subcommand(), active_subcommand_end_pos(), max_subcommand_chain_count()
        {
            subcommands.insert({"", std::unordered_map<std::string, int>()}); // for when no active_subcommand is provided
        }

        CLI(int argc, char** argv) 
        : args(), subcommands(), active_subcommand(), active_subcommand_end_pos(), max_subcommand_chain_count()
        {
            setArguments(argc, argv);
        }

        CLI(const std::vector<std::string>& args) 
        : args(), subcommands(), active_subcommand(), active_subcommand_end_pos(), max_subcommand_chain_count()
        {
            setArguments(args);
        }

        // Getters
        std::string getProgramName() const
        {
            if(args.empty()) {
                throw CLIException(__func__, "Argument list is empty");
            }

            std::string name;
            int start = 0;
            int end = args[0].size()-1;
            for(int i = args[0].size()-1; i >= 0; i--) {
                if(args[0][i] == '.' && end >= args[0].size()-1) {
                    end = i - 1;
                } else if(args[0][i] == '\\' || args[0][i] == '/') {
                    start = i + 1;
                    break;
                }
            }

            for(int i = start; i <= end; i++) {
                name.push_back(args[0][i]);
            }

            return name;
        }

        const std::vector<std::string>& getArguments() const
        {
            return args;
        }

        std::vector<std::string> getArguments(int start) const
        {
            return getArguments(start, args.size()-1);
        }

        std::vector<std::string> getArguments(int start, int end) const
        {
            if(start < 0 || start >= args.size()) {
                throw CLIException(__func__, "Index is out of range");
            }

            if(end < 0 || end >= args.size()) {
                throw CLIException(__func__, "Index is out of range");
            } 

            std::vector<std::string> result;
            for(int i = start; i <= end; i++) {
                result.push_back(args[i]);
            }

            return result;
        }

        int getArgumentCount() const
        {
            return args.size();
        }

        const std::string& getActiveSubcommand() const
        {
            return active_subcommand;
        }

        const std::string& getArgumentAt(int index) const
        {
            if(index < 0 || index >= args.size()) {
                throw CLIException(__func__, "Index is out of range");
            }

            return args[index];
        }

        std::string getArgumentAt(int start, int end) const
        {
            if(start < 0 || start >= args.size()) {
                throw CLIException(__func__, "Index is out of range");
            }

            if(end < 0 || end >= args.size()) {
                throw CLIException(__func__, "Index is out of range");
            } 

            std::string result;
            for(int i = start; i <= end; i++) {
                result.append(args[i]);
                if(i < end) {
                    result.push_back(' ');
                }
            }

            return result;
        }

        int getStartPosition() const
        {
            return active_subcommand_end_pos + 1;
        }

        int getFlagPosition(const std::string& flag) const
        {
            if(subcommands.at(active_subcommand).count(flag) < 1) {
                throw CLIException(__func__, "\"" + flag + "\" is not a valid flag");
            }
            return subcommands.at(active_subcommand).at(flag);
        }

        std::unordered_set<std::string> getValidSubcommands() const
        {
            std::unordered_set<std::string> valid_subcommands;
            for(const auto& i : subcommands) {
                valid_subcommands.insert(i.first);
            }
            return valid_subcommands;
        }

        std::unordered_set<std::string> getValidFlags(std::string subcmd = "") const
        {
            if(subcmd.empty()) {
                subcmd = active_subcommand;
            }

            if(!isValidSubcommand(subcmd)) {
                throw CLIException(__func__, "\"" + subcmd + "\" is not a valid subcommand");
            }

            std::unordered_set<std::string> flags;
            for(const auto& i : subcommands.at(subcmd)) {
                flags.insert(i.first);
            }

            return flags;
        }

        std::unordered_set<std::string> getActiveFlags() const
        {
            std::unordered_set<std::string> flags;
            for(const auto& i : subcommands.at(active_subcommand)) {
                if(i.second >= 0) {
                    flags.insert(i.first);
                }
            }

            return flags;
        }

        std::unordered_map<std::string, int> getActiveFlagsAndPositions() const
        {
            std::unordered_map<std::string, int> flags;
            for(const auto& i : subcommands.at(active_subcommand)) {
                if(i.second >= 0) {
                    flags.insert(i);
                }
            }

            return flags;
        }

        std::string getActiveFlagIn(const std::vector<std::string>& flags) const
        {
            for(int i = 0; i < flags.size(); i++) {
                if(isFlagActive(flags[i])) {
                    return flags[i];
                }
            }

            return std::string();
        }

        std::vector<std::string> getAllActiveFlagsIn(const std::vector<std::string>& flags) const 
        {
            std::vector<std::string> active_flags;
            for(int i = 0; i < flags.size(); i++) {
                if(isFlagActive(flags[i])) {
                    active_flags.push_back(flags[i]);
                }
            }

            return active_flags;
        }

        std::string getAnyValue(int occurance = 1, const std::unordered_set<std::string>& excluded_flags = {}) const
        {
            return getAnyValue(excluded_flags, occurance);
        }

        std::string getAnyValue(const std::unordered_set<std::string>& excluded_flags, int occurance = 1) const
        {
            if(occurance < 1) {
                occurance = 1;
            }

            bool skip_val = false;
            int counter = 1;
            for(int i = getStartPosition(); i < args.size(); i++) {
                if(hasFlagPrefix(args[i])) {
                    std::vector<std::string> flags = splitFlags(trim(args[i]));
                    for(int j = 0; j < flags.size(); j++) {
                        if(excluded_flags.count(flags[j]) < 1) {
                            skip_val = false;
                            break;
                        }
                        skip_val = true;
                    }
                    continue;
                }

                if(skip_val) {
                    continue;
                }

                if(counter == occurance) {
                    return args[i];
                }
                counter++;
            }

            return std::string();
        }

        std::vector<std::string> getAllValues(int limit = -1)
        {
            return getAllValues(std::unordered_set<std::string>(), limit);
        }

        std::vector<std::string> getAllValues(const std::unordered_set<std::string>& excluded_flags, int limit = -1)
        {
            std::vector<std::string> result;
            bool skip_val = false;
            for(int i = getStartPosition(); i < args.size(); i++) {
                if(hasFlagPrefix(args[i])) {
                    std::vector<std::string> flags = splitFlags(trim(args[i]));
                    for(int j = 0; j < flags.size(); j++) {
                        if(excluded_flags.count(flags[j]) < 1) {
                            skip_val = false;
                            break;
                        }
                        skip_val = true;
                    }
                    continue;
                }

                if(!skip_val) {
                    result.push_back(args[i]);
                }

                if(limit >= 0 && result.size() >= limit) {
                    return result;
                }
            }

            return result;
        }

        std::string getValueOf(int occurance = 1) const
        {
            return getValueOf(active_subcommand, occurance);
        }

        std::string getValueOf(const std::initializer_list<std::string>& flag, int occurance = 1) const
        {
            for(const auto& i : flag) {
                if(isFlagActive(i)) {
                    return getValueOf(i, occurance);
                }
            }
            throw CLIException(__func__, "None of the flags are in the argument list");
        }

        std::string getValueOf(const std::vector<std::string>& flag, int occurance = 1) const
        {
            for(int i = 0; i < flag.size(); i++) {
                if(isFlagActive(flag[i])) {
                    return getValueOf(flag[i], occurance);
                }
            }
            throw CLIException(__func__, "None of the flags are in the argument list");
        }

        std::string getValueOf(const std::string& flag, int occurance = 1) const
        {
            if(occurance < 1) {
                occurance = 1;
            }

            int counter = 1;
            if(isValidFlag(flag)) {
                int flag_pos = subcommands.at(active_subcommand).at(flag);
                if(flag_pos < 0) {
                    throw CLIException(__func__, "\"" + flag + "\" is not in the argument list");
                }

                std::string temp = args[flag_pos];
                int equal = temp.find_first_of('=', 2);
                if(equal != std::string::npos) {
                    if(counter == occurance) {
                        return temp.substr(equal + 1, temp.size() - equal);
                    }
                    counter++;
                } 

                for(int i = flag_pos + 1; i < args.size(); i++) {
                    if(hasFlagPrefix(args[i])) {
                        break;
                    } else if(counter == occurance) {
                        return args[i];
                    }
                    counter++;
                }
            } else if(flag == active_subcommand) {
                for(int i = active_subcommand_end_pos + 1; i < args.size(); i++) {
                    if(hasFlagPrefix(args[i])) {
                        break;
                    } else if(counter == occurance) {
                        return args[i];
                    }
                    counter++;
                }
            } else {
                throw CLIException(__func__, "\"" + flag + "\" is neither an active flag or subcommand");
            }

            return std::string();
        }

        std::vector<std::string> getAllValuesOf(int limit = -1) const
        {
            return getAllValuesOf(active_subcommand, limit);
        }

        std::vector<std::string> getAllValuesOf(const std::initializer_list<std::string>& flag, int limit = -1) const
        {
            for(const auto& i : flag) {
                if(isFlagActive(i)) {
                    return getAllValuesOf(i, limit);
                }
            }
            throw CLIException(__func__, "None of the flags is in the argument list");
        }

        std::vector<std::string> getAllValuesOf(const std::vector<std::string>& flag, int limit = -1) const
        {
            for(int i = 0; i < flag.size(); i++) {
                if(isFlagActive(flag[i])) {
                    return getAllValuesOf(flag[i], limit);
                }
            }
            throw CLIException(__func__, "None of the flags is in the argument list");
        }

        std::vector<std::string> getAllValuesOf(const std::string& flag, int limit = -1) const
        {
            std::vector<std::string> values;
            if(limit == 0) {
                return values;
            }

            if(isValidFlag(flag)) {
                int flag_pos = subcommands.at(active_subcommand).at(flag);
                if(flag_pos < 0) {
                    throw CLIException(__func__, "Flag \"" + flag + "\" is not in the argument list");
                }

                std::string temp = args[flag_pos];
                int equal = temp.find_first_of("=");
                if(equal != std::string::npos) {
                    values.push_back(temp.substr(equal + 1, temp.size() - equal));
                } 
                
                for(int i = flag_pos + 1; i < args.size(); i++) {
                    if(hasFlagPrefix(args[i]) || limit > 0 && values.size() >= limit) {
                        break;
                    }
                    values.push_back(args[i]);
                }
            } else if(flag == active_subcommand) {
                for(int i = active_subcommand_end_pos + 1; i < args.size(); i++) {
                    if(hasFlagPrefix(args[i]) || limit > 0 && values.size() >= limit) {
                        break;
                    }
                    values.push_back(args[i]);
                }
            } else {
                throw CLIException(__func__, "\"" + flag + "\" is neither an active flag or subcommand");
            }
            return values;
        }

        std::unordered_map<std::string, std::vector<std::string>> getActiveFlagsAndValues() const
        {
            std::unordered_map<std::string, std::vector<std::string>> values;
            values.insert({"", getAllValuesOf(active_subcommand)});
            for(const auto& i : subcommands.at(active_subcommand)) {
                if(i.second >= 0) {
                    values.insert({i.first, getAllValuesOf(i.first)});
                }
            }

            return values;
        }

        // Setters
        void setArguments(int argc, char** argv)
        {
            if(argc > 0) {
                setArguments(std::vector<std::string>(argv, argv+argc));
            }
        }

        void setArguments(const std::vector<std::string>& args)
        {
            this->args = args;
            if(subcommands.empty()) {
                subcommands.insert({"", std::unordered_map<std::string, int>()});
            }
        }

        // Checkers
        bool isValidSubcommand(const std::string& subcmd) const
        {
            return subcommands.count(subcmd) > 0;
        }

        bool isActiveSubcommand(const std::string& subcmd = "") const 
        {
            return active_subcommand == subcmd;
        }

        bool noActiveSubcommand() const 
        {
            return active_subcommand.empty();
        }

        bool isFlagActive(const std::string& flag) const
        {
            if(subcommands.at(active_subcommand).count(flag) < 1) {
                throw CLIException(__func__, "\"" + flag + "\" is not a valid flag of \"" + active_subcommand + "\"");
            }
            return subcommands.at(active_subcommand).at(flag) >= 0;
        }

        bool isFlagActive(const std::initializer_list<std::string>& flag) const
        {
            for(const auto& i : flag) {
                if(isFlagActive(i)) {
                    return true;
                }
            }
            return false;
        }

        bool isFlagActive(const std::vector<std::string>& flag) const
        {
            for(int i = 0; i < flag.size(); i++) {
                if(isFlagActive(flag[i])) {
                    return true;
                }
            }
            return false;
        }

        bool areFlagsActive(const std::vector<std::string>& flag) const
        {
            for(int i = 0; i < flag.size(); i++) {
                if(!isFlagActive(flag[i])) {
                    return false;
                }
            }
            return true;
        }

        bool isValidFlag(const std::string& flag) const 
        {
            return subcommands.at(active_subcommand).count(flag) > 0;
        }

        bool isValidFlag(const std::string& subcmd, const std::string& flag) const // checks if flag is valid
        {
            if(!isValidSubcommand(subcmd)) {
                throw CLIException(__func__, "\"" + subcmd + "\" is not a valid subcommand");
            }
            return subcommands.at(subcmd).count(flag) > 0;
        }

        // Modifiers
        void init()
        {
            setActiveSubcommand();
            initFlags(active_subcommand);
        }

        void addSubcommands(const std::vector<std::string>& valid_subs)
        {
            setSubcommands(valid_subs, false);
        }

        void addGlobalFlags(const std::vector<std::string>& valid_flags)
        {

        }

        void addFlags(const std::vector<std::string>& valid_flags)
        {
            setFlags("", valid_flags, false);
        }

        void addFlags(const std::string& subcmd, const std::vector<std::string>& valid_flags)
        {
            setFlags(subcmd, valid_flags, false);
        }
        
        void clear()
        {
            args.clear();
            clearSubcommands();
        }

        void clearSubcommands()
        {
            subcommands.clear();
            subcommands.insert({"", std::unordered_map<std::string, int>()});
            active_subcommand.clear();
            active_subcommand_end_pos = 0;
            max_subcommand_chain_count = 0;
        }

        void clearFlags(const std::string& subcmd = "")
        {
            if(!isValidSubcommand(subcmd)) {
                throw CLIException(__func__, "\"" + subcmd + "\" is not a valid subcommand");
            }
            subcommands.at(subcmd).clear();
        }
};