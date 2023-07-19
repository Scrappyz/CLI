#include "cli.hpp"
#include <typeinfo>
#include <gtest/gtest.h>

using namespace std;

TEST(setArguments, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "origin", "-h", "-o"});
    unordered_set<string> expected_sub = {""};
    EXPECT_EQ(cli.getValidSubcommands(), expected_sub);
    EXPECT_EQ(cli.getActiveSubcommand(), "");
    //EXPECT_EQ(cli.getMaxSubcommandChainCount(), 0);
    cli.addSubcommands({"remote", "add", "origin"});
    EXPECT_EQ(cli.getActiveSubcommand(), "remote");
    EXPECT_EQ(cli.getActiveSubcommand(), "remote");
    cli.addSubcommands({"remote", "remote add origin"});
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add origin");

    cli.addSubcommands({"add", "origin", "remote add origin"});
    cli.setArguments({"MyProgram", "add", "origin"});
    EXPECT_EQ(cli.getActiveSubcommand(), "add");

    cli.addSubcommands({"add", "origin", "remote add origin"});
    cli.setArguments({"MyProgram", "remote", "add", "origin"});
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add origin");

    cli.setArguments({"MyProgram", "add", "hi"});
    EXPECT_EQ(cli.getActiveSubcommand(), "add");
    cli.addSubcommands({"add", "hi"});
    EXPECT_EQ(cli.getActiveSubcommand(), "add");

    cli.addSubcommands({""});
    EXPECT_EQ(cli.getActiveSubcommand(), "");

    cli.setArguments({"MyProgram", "remote", "-h"});
    cli.addSubcommands({"remote", "remote add"});
    cli.addFlags("remote", {"-h"});
    cli.addFlags("remote add", {"--help"});
    EXPECT_EQ(cli.getActiveSubcommand(), "remote");
    EXPECT_EQ(cli.isFlagActive("-h"), true);
    cli.setArguments({"MyProgram", "remote", "add", "--help"});
    EXPECT_THROW(cli.isFlagActive("-h"), CLIException);
    EXPECT_EQ(cli.isFlagActive("--help"), true);
}

TEST(addSubcommands, general)
{
    CLI cli({"MyProgram", "remote", "add", "--origin"});
    cli.addSubcommands({"pull", "push", "remote add"});
    //EXPECT_EQ(cli.getMaxSubcommandChainCount(), 2);

    cli.addSubcommands({"", ""});
    //EXPECT_EQ(cli.getMaxSubcommandChainCount(), 0);

    cli.addSubcommands({"", "  hello", "   hi   ", "boi   "});
    unordered_set<string> expected_subcmd = {"", "hello", "hi", "boi"};
    //EXPECT_EQ(cli.getMaxSubcommandChainCount(), 1);
    EXPECT_EQ(cli.getValidSubcommands(), expected_subcmd);

    CLI cmd({"MyProgram"});
    expected_subcmd = {""};
    cmd.addSubcommands({});
    EXPECT_EQ(cmd.getValidSubcommands(), expected_subcmd);
}

TEST(addFlags, general)
{
    CLI cli;
    unordered_set<string> expected_sub;
    cli.setArguments({"MyProgram", "push", "-f", "all"});
    cli.addSubcommands({"push"});
    cli.addFlags("push", {"-f"});
    EXPECT_EQ(cli.isValidFlag("-f"), true);
    EXPECT_EQ(cli.isFlagActive("-f"), true);

    cli.addSubcommands({"pull", "pull jim hoe"});
    expected_sub = {"pull", "pull jim hoe", ""};
    EXPECT_EQ(cli.getValidSubcommands(), expected_sub);
    EXPECT_THROW(cli.addFlags("push", {"-f"}), CLIException);
    EXPECT_EQ(cli.getActiveSubcommand(), "");

    cli.setArguments({"MyProgram", "pull", "jim", "hoe", "-g", "value"});
    EXPECT_EQ(cli.getActiveSubcommand(), "pull jim hoe");
    EXPECT_THROW(cli.addFlags("pull jim hoe", {"-f"}), CLIException);
    EXPECT_EQ(cli.isValidFlag("-f"), true);
    EXPECT_EQ(cli.isFlagActive("-f"), false);
}

TEST(addFlags, multiple_single_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hmv", "--help", "stuff"});
    cli.addSubcommands({});
    cli.addFlags({"-h", "--help", "-m", "-v"});
    EXPECT_EQ(cli.getFlagPosition("-h"), 1);
    EXPECT_EQ(cli.getFlagPosition("-m"), 1);
    EXPECT_EQ(cli.getFlagPosition("-v"), 1);
    EXPECT_EQ(cli.getFlagPosition("--help"), 2);

    cli.clear();
    cli.setArguments({"MyProgram", "-hmv", "-gc", "--hello"});
    cli.addFlags({"-h", "-m", "-v", "-g", "-c", "--hello"});
    EXPECT_EQ(cli.getFlagPosition("-h"), 1);
    EXPECT_EQ(cli.getFlagPosition("-m"), 1);
    EXPECT_EQ(cli.getFlagPosition("-v"), 1);
    EXPECT_EQ(cli.getFlagPosition("-g"), 2);
    EXPECT_EQ(cli.getFlagPosition("-c"), 2);
    EXPECT_EQ(cli.getFlagPosition("--hello"), 3);

    cli.clear();
    cli.setArguments({"MyProgram", "-hmvx", "-gc", "--hello"});
    EXPECT_THROW(cli.addFlags({"-h", "-m", "-v", "-g", "-c", "--hello"}), CLIException);
}

TEST(addFlags, duplicate_multiple_single_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-uvv", "hello"});
    cli.addFlags({"-h", "-u", "-v"});
    EXPECT_EQ(cli.getFlagPosition("-h"), -1);
    EXPECT_EQ(cli.getFlagPosition("-u"), 1);
    EXPECT_EQ(cli.getFlagPosition("-v"), 1);

    cli.setArguments({"MyProgram", "-uvv", "-v", "hello"});
    EXPECT_EQ(cli.getFlagPosition("-v"), 2);
}

TEST(addFlags, substring_in_another_flag)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hmv", "--helpme", "stuff"});
    cli.addSubcommands({});
    cli.addFlags({"-h", "--help", "-m", "-v", "--helpme"});
    EXPECT_EQ(cli.getFlagPosition("-h"), 1);
    EXPECT_EQ(cli.getFlagPosition("-m"), 1);
    EXPECT_EQ(cli.getFlagPosition("-v"), 1);
    EXPECT_EQ(cli.getFlagPosition("--help"), -1);
    EXPECT_EQ(cli.getFlagPosition("--helpme"), 2);

    cli.setArguments({"MyProgram", "--help", "--helpme", "stuff"});
    EXPECT_EQ(cli.getFlagPosition("--help"), 1);
    EXPECT_EQ(cli.getFlagPosition("--helpme"), 2);

    cli.setArguments({"MyProgram", "--helpme", "--help", "stuff"});
    EXPECT_EQ(cli.getFlagPosition("--help"), 2);
    EXPECT_EQ(cli.getFlagPosition("--helpme"), 1);
}

TEST(addFlags, duplicate_flags_initialization)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--help", "-h", "stuff"});
    cli.addFlags({"-h", "--help", "-h", "--help"});
    unordered_map<string, int> expected_flags = {{"-h", 2}, {"--help", 1}};
    EXPECT_EQ(cli.getActiveFlagsAndPositions(), expected_flags);
}

TEST(addFlags, duplicate_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--help", "--help"});
    cli.addFlags({"--help"});
    EXPECT_EQ(cli.getFlagPosition("--help"), 2);

    cli.clear();
    cli.setArguments({"MyProgram", "--flag=value", "--flag"});
    cli.addFlags({"--flag"});
    EXPECT_EQ(cli.getFlagPosition("--flag"), 2);
    EXPECT_EQ(cli.getAllValuesOf("--flag"), vector<string>());
}

TEST(addFlags, flags_with_invalid_prefix)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--help", "-h", "stuff"});
    EXPECT_THROW(cli.addFlags({"--help", "/h"}), CLIException);
    EXPECT_THROW(cli.addFlags({"---help"}), CLIException);
    EXPECT_THROW(cli.addFlags({"money"}), CLIException);
}

TEST(addFlags, multiple_single_flag_initialization)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hmv"});
    EXPECT_THROW(cli.addFlags({"-hmv"}), CLIException);
    EXPECT_THROW(cli.addFlags({"-help"}), CLIException);
}

TEST(addFlags, invalid_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hel-lo", "stuff"});
    EXPECT_THROW(cli.addFlags({"-h", "-e", "-o"}), CLIException);

    cli.clear();
    cli.addFlags({"-h", "-e", "-o"});
    EXPECT_THROW(cli.setArguments({"MyProgram", "-hel-lo", "stuff"}), CLIException);
}

TEST(addFlags, edge_cases)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--help", "-h", "stuff"});
    EXPECT_THROW(cli.addFlags({""}), CLIException);
    EXPECT_THROW(cli.addFlags({"-"}), CLIException);
    EXPECT_THROW(cli.addFlags({"--"}), CLIException);
    EXPECT_THROW(cli.addFlags({"---"}), CLIException);
}

TEST(initialization, order)
{
    // initialization order test
    vector<string> expected_args = {"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"};
    unordered_set<string> expected_subs = {"remote", "remote add", ""};
    unordered_map<string, int> expected_flags = {{"-h", 3}, {"--output", 5}};

    // args -> subcommands -> flags
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"});
    cli.addSubcommands({"remote", "remote add"});
    cli.addFlags("remote add", {"-h", "--output", "-o"});
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add");
    EXPECT_EQ(cli.getValidSubcommands(), expected_subs);
    EXPECT_EQ(cli.getActiveFlagsAndPositions(), expected_flags);

    // subcommands -> args -> flags
    cli.clear();
    cli.addSubcommands({"remote", "remote add"});
    cli.setArguments({"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"});
    cli.addFlags("remote add", {"-h", "--output", "-o"});
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add");
    EXPECT_EQ(cli.getValidSubcommands(), expected_subs);
    EXPECT_EQ(cli.getActiveFlagsAndPositions(), expected_flags);

    // flags -> subcommands -> args
    cli.clear();
    expected_flags.clear();
    EXPECT_THROW(cli.addFlags("remote add", {"-h", "--output", "-o"}), CLIException); // subcommands have not been set yet
    cli.addSubcommands({"remote", "remote add"});
    cli.setArguments({"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"});
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add");
    EXPECT_EQ(cli.getValidSubcommands(), expected_subs);
    EXPECT_EQ(cli.getActiveFlagsAndPositions(), expected_flags);

    // flags -> args -> subcommands
    cli.clear();
    EXPECT_THROW(cli.addFlags("remote add", {"-h", "--output", "-o"}), CLIException);
    cli.setArguments({"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"});
    cli.addSubcommands({"remote", "remote add"});
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add");
    EXPECT_EQ(cli.getValidSubcommands(), expected_subs);
    EXPECT_EQ(cli.getActiveFlagsAndPositions(), expected_flags);
}

TEST(getProgramName, general)
{
    CLI cli;
    cli.setArguments({"myprogram", "subcommand"});
    EXPECT_EQ(cli.getProgramName(), "myprogram");
    cli.setArguments({"myprogram.exe", "subcommand"});
    EXPECT_EQ(cli.getProgramName(), "myprogram");
    cli.setArguments({"path/to/myprogram", "subcommand"});
    EXPECT_EQ(cli.getProgramName(), "myprogram");
    cli.setArguments({"path/to/myprogram.exe", "subcommand"});
    EXPECT_EQ(cli.getProgramName(), "myprogram");
    cli.setArguments({"my.program.exe"});
    EXPECT_EQ(cli.getProgramName(), "my.program");
}

TEST(getArguments, general)
{
    vector<string> expected_args = {"MyProgram", "remote", "add", "-R", "my repo"};
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "-R", "my repo"});
    cli.addSubcommands({"remote add"});
    cli.addFlags("remote add", {"-R"});

    EXPECT_EQ(cli.getArguments(), expected_args);
    expected_args = {"remote", "add", "-R", "my repo"};
    EXPECT_EQ(cli.getArguments(1), expected_args);
    expected_args = {"remote", "add", "-R"};
    EXPECT_EQ(cli.getArguments(1, 3), expected_args);
}

TEST(getArgumentAt, concatenation)
{
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "--help", "value", "lol"});
    EXPECT_EQ(cli.getArgumentAt(0), "MyProgram");
    EXPECT_THROW(cli.getArgumentAt(2, 6), CLIException);
    EXPECT_THROW(cli.getArgumentAt(-1, 2), CLIException);
    EXPECT_THROW(cli.getArgumentAt(6, 2), CLIException);
    EXPECT_EQ(cli.getArgumentAt(0, 2), "MyProgram remote add");
    EXPECT_EQ(cli.getArgumentAt(3, 5), "--help value lol");
    EXPECT_EQ(cli.getArgumentAt(3, 2), "");
}

TEST(getActiveFlagIn, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-h", "hi", "-v"});
    cli.addFlags({"-h", "--help", "-v", "--verbose"});
    EXPECT_EQ(cli.getActiveFlagIn({"-h", "--help"}), "-h");
    EXPECT_EQ(cli.getActiveFlagIn({"-v", "--verbose"}), "-v");
}

TEST(getAllActiveFlagsIn, general)
{
    vector<string> expected_flags = {"-h", "--help", "-v"};

    CLI cli;
    cli.setArguments({"MyProgram", "-h", "--help", "-v", "hi"});
    cli.addFlags({"-h", "--help", "-v", "--verbose"});
    EXPECT_EQ(cli.getAllActiveFlagsIn({"-h", "--help", "-v"}), expected_flags);
    EXPECT_EQ(cli.getAllActiveFlagsIn({"-h", "--help", "-v", "--verbose"}), expected_flags);
}

TEST(getAllActiveFlagsIn, multiple_flags)
{
    vector<string> expected_flags = {"-h", "--help"};
    
    CLI cli;
    cli.setArguments({"MyProgram", "-h", "-h", "--help"});
    cli.addFlags({"-h", "--help", "-v", "--verbose"});
    EXPECT_EQ(cli.getAllActiveFlagsIn({"-h", "--help", "-v", "--verbose"}), expected_flags);
}

TEST(getAnyValue, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "init", "cpp-app", "--path", "value1", "-f", "trueval"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"--path", "-f"});
    EXPECT_EQ(cli.getAnyValue({"--path"}), "cpp-app");
    EXPECT_EQ(cli.getAnyValue(2, {"--path"}), "trueval");

    cli.clear();
    cli.setArguments({"MyProgram", "init", "--path", "my/path", "cpp-app"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"--path", "-f"});
    EXPECT_EQ(cli.getAnyValue(), "my/path");
    EXPECT_EQ(cli.getAnyValue(2), "cpp-app");
    EXPECT_EQ(cli.getAnyValue({"--path"}), "");
}

TEST(getAnyValue, multiple_single_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "init", "-hiv", "value1", "-o", "value2"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"-h", "-i", "-v", "-o"});
    EXPECT_EQ(cli.getAnyValue({"-h"}), "value1");
    EXPECT_EQ(cli.getAnyValue({"-i"}), "value1");
    EXPECT_EQ(cli.getAnyValue({"-v"}), "value1");
    EXPECT_EQ(cli.getAnyValue({"-h", "-i", "-v"}), "value2");
}

TEST(getAllValues, general)
{
    vector<string> expected_vals = {"cpp-app", "value1", "trueval"};

    CLI cli;
    cli.setArguments({"MyProgram", "init", "cpp-app", "--path", "value1", "-f", "trueval"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"--path", "-f"});
    EXPECT_EQ(cli.getAllValues(), expected_vals);
    expected_vals = {"cpp-app", "trueval"};
    EXPECT_EQ(cli.getAllValues({"--path"}), expected_vals);
    expected_vals = {"cpp-app"};
    EXPECT_EQ(cli.getAllValues(1), expected_vals);
}

TEST(getAllValues, multiple_single_flags)
{
    vector<string> expected_vals = {"value1", "value2"};

    CLI cli;
    cli.setArguments({"MyProgram", "init", "-hiv", "value1", "-o", "value2"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"-h", "-i", "-v", "-o"});
    EXPECT_EQ(cli.getAllValues({"-h"}), expected_vals);
    EXPECT_EQ(cli.getAllValues({"-i"}), expected_vals);
    EXPECT_EQ(cli.getAllValues({"-v"}), expected_vals);
    expected_vals = {"value2"};
    EXPECT_EQ(cli.getAllValues({"-h", "-i", "-v"}), expected_vals);
}

TEST(getValueOf, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--flag=value"});
    cli.addFlags({"--flag"});
    EXPECT_EQ(cli.getValueOf("--flag"), "value");
    EXPECT_EQ(cli.getValueOf("--flag", 2), "");
}

TEST(getValueOf, multiple_values)
{
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "val1", "val2", "--flag", "fval1", "fval2", "-h", "hval1", "hval2", "hval3",
    "-m=mval1", "mval2", "mval3", "-f", "ffval1", "ffval2"});
    cli.addSubcommands({"remote add"});
    cli.addFlags("remote add", {"--flag", "-h", "-m", "-f"});
    EXPECT_EQ(cli.getValueOf(), "val1");
    EXPECT_EQ(cli.getValueOf(1), "val1");
    EXPECT_EQ(cli.getValueOf(2), "val2");
    EXPECT_EQ(cli.getValueOf(3), "");
    EXPECT_EQ(cli.getValueOf("--flag"), "fval1");
    EXPECT_EQ(cli.getValueOf("--flag", 2), "fval2");
    EXPECT_EQ(cli.getValueOf("-h"), "hval1");
    EXPECT_EQ(cli.getValueOf("-h", 2), "hval2");
    EXPECT_EQ(cli.getValueOf("-h", 3), "hval3");
    EXPECT_EQ(cli.getValueOf("-m"), "mval1");
    EXPECT_EQ(cli.getValueOf("-m", 2), "mval2");
    EXPECT_EQ(cli.getValueOf("-m", 3), "mval3");
    EXPECT_EQ(cli.getValueOf({"-f", "--flag"}), "ffval1");
    EXPECT_EQ(cli.getValueOf({"--flag", "-f"}), "fval1");

    cli.clear();
    cli.setArguments({"MyProgram", "-hmv=value", "value1", "value2"});
    cli.addFlags({"-h", "-m", "-v"});
    EXPECT_EQ(cli.getValueOf("-h"), "value");
    EXPECT_EQ(cli.getValueOf("-m"), "value");
    EXPECT_EQ(cli.getValueOf("-v"), "value");
    EXPECT_EQ(cli.getValueOf("-h", 2), "value1");
    EXPECT_EQ(cli.getValueOf("-m", 2), "value1");
    EXPECT_EQ(cli.getValueOf("-v", 2), "value1");
}

TEST(getValueOf, multiple_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--flag=value1", "--flag=value2"});
    cli.addFlags({"--flag"});
    EXPECT_EQ(cli.getValueOf("--flag"), "value2");
}

TEST(getValueOf, no_value)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hmv"});
    cli.addFlags({"-h", "-m", "-v"});
}

TEST(getAllValuesOf, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--flag=value"});
    cli.addFlags({"--flag"});
    vector<string> flag_val = {"value"};
    EXPECT_EQ(cli.getAllValuesOf("--flag"), flag_val);
}

TEST(getAllValuesOf, multiple_values)
{
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "val1", "val2", "--flag", "fval1", "fval2", "-h", "hval1", "hval2", "hval3",
    "-m=mval1", "mval2", "mval3", "-f", "ffval1", "ffval2"});
    cli.addSubcommands({"remote add"});
    cli.addFlags("remote add", {"--flag", "-h", "-m", "-f"});
    vector<string> expected_values = {"val1", "val2"};
    EXPECT_EQ(cli.getAllValuesOf("remote add"), expected_values);
    expected_values = {"fval1", "fval2"};
    EXPECT_EQ(cli.getAllValuesOf("--flag"), expected_values);
    expected_values = {"hval1", "hval2"};
    EXPECT_EQ(cli.getAllValuesOf("-h", 2), expected_values);
    expected_values = {"mval1", "mval2", "mval3"};
    EXPECT_EQ(cli.getAllValuesOf("-m"), expected_values);
    expected_values = {"mval1"};
    EXPECT_EQ(cli.getAllValuesOf("-m", 1), expected_values);
    expected_values = {"ffval1", "ffval2"};
    EXPECT_EQ(cli.getAllValuesOf({"-f", "--flag"}), expected_values);
    expected_values = {"ffval1"};
    EXPECT_EQ(cli.getAllValuesOf({"-f", "--flag"}, 1), expected_values);
    expected_values = {"fval1", "fval2"};
    EXPECT_EQ(cli.getAllValuesOf({"--flag", "-f"}), expected_values);
    expected_values = {"fval1"};
    EXPECT_EQ(cli.getAllValuesOf({"--flag", "-f"}, 1), expected_values);
}

TEST(getAllValuesOf, no_value)
{
    CLI cli;
    cli.setArguments({"MyProgram", "add"});
    cli.addSubcommands({"add"});
    vector<string> expected_values;
    EXPECT_EQ(cli.getAllValuesOf("add"), expected_values);

    cli.setArguments({"MyProgram", "add", "val1"});
    expected_values = {};
    EXPECT_EQ(cli.getAllValuesOf("add", 0), expected_values);

    cli.setArguments({"MyProgram", "add", "-f", "val1"});
    cli.addFlags({"-f"});
    expected_values = {};
    EXPECT_EQ(cli.getAllValuesOf("-f", 0), expected_values);
}

TEST(getActiveFlagsAndValues, general)
{
    unordered_map<string, vector<string>> expected_values;
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "val1", "val2", "--flag=fval1", "fval2", "-h", "hval1", "hval2"});
    cli.addSubcommands({"remote add", "remote"});
    cli.addFlags("remote add", {"--flag", "-h", "--help"});

    expected_values = {{"", {"val1", "val2"}}, {"--flag", {"fval1", "fval2"}}, {"-h", {"hval1", "hval2"}}};
    EXPECT_EQ(cli.getActiveFlagsAndValues(), expected_values);

    cli.setArguments({"MyProgram", "val1", "val2", "--flag=fval1", "fval2", "-h", "hval1", "hval2"});
    cli.addFlags({"--flag", "-h", "--help"});
    EXPECT_EQ(cli.getActiveFlagsAndValues(), expected_values);

    cli.setArguments({"MyProgram", "--flag=fval1", "fval2", "-h", "hval1", "hval2"});
    expected_values = {{"", {}}, {"--flag", {"fval1", "fval2"}}, {"-h", {"hval1", "hval2"}}};
    EXPECT_EQ(cli.getActiveFlagsAndValues(), expected_values);
}

TEST(checkers, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "home", "-g"});
    EXPECT_EQ(cli.isValidFlag("-g"), false);
    EXPECT_THROW(cli.isFlagActive("-g"), CLIException);
    EXPECT_THROW(cli.getFlagPosition("-g"), CLIException);

    cli.addFlags({"-g"});
    EXPECT_EQ(cli.isValidSubcommand("home"), false);
    EXPECT_EQ(cli.isValidFlag("-g"), true);
    EXPECT_EQ(cli.isFlagActive("-g"), true);
    EXPECT_EQ(cli.getFlagPosition("-g"), 2);

    cli.addSubcommands({"home", "init"}); // will reset valid flags
    EXPECT_EQ(cli.getActiveSubcommand(), "home");
    EXPECT_EQ(cli.isValidFlag("-g"), false);
    EXPECT_THROW(cli.isFlagActive("-g"), CLIException); // because "-g" is not a valid flag of "home"
    EXPECT_THROW(cli.getFlagPosition("-g"), CLIException);

    EXPECT_THROW(cli.addFlags("test", {"--help"}), CLIException);
    cli.addFlags("home", {"-g", "--help"});
    cli.setArguments({"MyProgram", "home", "-g", "--help"});
    EXPECT_EQ(cli.getActiveSubcommand(), "home");
    EXPECT_EQ(cli.isValidFlag("-g"), true);
    EXPECT_EQ(cli.isValidFlag("--help"), true);
    EXPECT_EQ(cli.isFlagActive("-g"), true);
    EXPECT_EQ(cli.isFlagActive("--help"), true);
    EXPECT_EQ(cli.getFlagPosition("-g"), 2);
    EXPECT_EQ(cli.getFlagPosition("--help"), 3);

    cli.setArguments({"MyProgram", "init", "-g", "this"});
    EXPECT_EQ(cli.getActiveSubcommand(), "init");
    EXPECT_EQ(cli.isValidFlag("-g"), false);
    EXPECT_THROW(cli.isFlagActive("-g"), CLIException);
    EXPECT_THROW(cli.getFlagPosition("-g"), CLIException);

    cli.addFlags("init", {"-g"});
    EXPECT_EQ(cli.isValidFlag("-g"), true);
    EXPECT_EQ(cli.isFlagActive("-g"), true);
    EXPECT_EQ(cli.getFlagPosition("-g"), 2);

    EXPECT_THROW(cli.addFlags("init", {"-h"}), CLIException); // "-h" is not in argument list
}

TEST(isFlagActive, multiple_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "-us", "hello", "--substring", "hi"});
    cli.addFlags({"-u", "--unscramble", "-s", "--substring"});
    EXPECT_EQ(cli.isFlagActive({"-u", "-s", "--substring"}), true);
    EXPECT_EQ(cli.isFlagActive({"-u", "--unscramble"}), true);
    EXPECT_EQ(cli.isFlagActive({"--unscramble", "--substring"}), true);
    EXPECT_EQ(cli.areFlagsActive({"-u", "-s", "--substring"}), true);
    EXPECT_EQ(cli.areFlagsActive({"-u", "--unscramble", "-s", "--substring"}), false);
}

TEST(clear, general)
{
    vector<string> expected_args;
    unordered_set<string> expected_subs = {""};
    string expected_active_sub;
    int expected_max_chain = 0;

    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "-h", "hello"});
    cli.addSubcommands({"remote", "remote add", "status"});
    cli.addFlags({"-h", "--help", "--force"});
    cli.clear();
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getValidSubcommands(), expected_subs);
    EXPECT_EQ(cli.getActiveSubcommand(), expected_active_sub);
}