#include <iostream>
#include "cli.hpp"

void print(const CLI& cli)
{
    std::cout << "Active Subcommand: \"" << cli.getActiveSubcommand() << "\"" << std::endl;
    std::unordered_set<std::string> active_flags = cli.getActiveFlags();

    if(active_flags.empty()) {
        std::cout << "[NO ACTIVE FLAGS]" << std::endl;
        return;
    }

    std::cout << "Active Flags: ";
    for(const auto& i : active_flags) {
        std::cout << "\"" + i << "\" "; 
    }
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.addSubcommands({"test"});
        cli.init();
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}