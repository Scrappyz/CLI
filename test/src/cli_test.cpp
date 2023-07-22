#include "cli.hpp"
#include <typeinfo>
#include <gtest/gtest.h>

using namespace std;

TEST(setArguments, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "origin", "-h", "-o"});
    unordered_set<string> expected_sub = {""};
    cli.init();
    EXPECT_EQ(cli.getSubcommands(true), expected_sub);
    EXPECT_EQ(cli.getActiveSubcommand(), "");
    cli.addSubcommands({"remote", "add", "origin"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "remote");
    EXPECT_EQ(cli.getActiveSubcommand(), "remote");

    cli.clearSubcommands();
    cli.addSubcommands({"remote", "remote add origin"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add origin");

    cli.clearSubcommands();
    cli.addSubcommands({"add", "origin", "remote add origin"});
    cli.setArguments({"MyProgram", "add", "origin"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "add");

    cli.clearSubcommands();
    cli.addSubcommands({"add", "origin", "remote add origin"});
    cli.setArguments({"MyProgram", "remote", "add", "origin"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add origin");

    cli.setArguments({"MyProgram", "add", "hi"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "add");
    cli.addSubcommands({"add", "hi"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "add");

    cli.clearSubcommands();
    cli.addSubcommands({""});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "");

    cli.setArguments({"MyProgram", "remote", "-h"});
    cli.addSubcommands({"remote", "remote add"});
    cli.addFlags("remote", {"-h"});
    cli.addFlags("remote add", {"--help"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "remote");
    EXPECT_EQ(cli.isFlagActive("-h"), true);
    cli.setArguments({"MyProgram", "remote", "add", "--help"});
    cli.init();
    EXPECT_THROW(cli.isFlagActive("-h"), CLIException);
    EXPECT_EQ(cli.isFlagActive("--help"), true);
}

TEST(addSubcommands, general)
{
    unordered_set<string> expected_subcmd = {"pull", "push", "remote add", ""};

    CLI cli({"MyProgram", "remote", "add", "--origin"});
    cli.addSubcommands({"pull", "push", "remote add"});
    EXPECT_EQ(cli.getSubcommands(true), expected_subcmd);

    cli.addSubcommands({"", ""});
    EXPECT_EQ(cli.getSubcommands(true), expected_subcmd);

    cli.addSubcommands({"", "  hello", "   hi   ", "boi   "});
    expected_subcmd = {"pull", "push", "remote add", "", "hello", "hi", "boi"};
    EXPECT_EQ(cli.getSubcommands(true), expected_subcmd);

    cli.clearSubcommands();
    cli.addSubcommands({"init", "add"});
    expected_subcmd = {"init", "add", ""};
    EXPECT_EQ(cli.getSubcommands(true), expected_subcmd);
}

TEST(removeSubcommands, general)
{
    unordered_set<string> expected_subs = {"init", "add", "remote add", ""};

    CLI cli;
    cli.addSubcommands({"init", "add", "remote add"});
    EXPECT_EQ(cli.getSubcommands(true), expected_subs);
    cli.removeSubcommands({"add", "remote add", ""});
    expected_subs = {"init", ""};
    EXPECT_EQ(cli.getSubcommands(true), expected_subs);
}

TEST(addFlags, general)
{
    CLI cli;
    unordered_set<string> expected_sub;
    cli.setArguments({"MyProgram", "push", "-f", "all"});
    cli.addSubcommands({"push"});
    cli.addFlags("push", {"-f"});
    cli.init();
    EXPECT_EQ(cli.isFlagValid("-f"), true);
    EXPECT_EQ(cli.isFlagActive("-f"), true);

    cli.addSubcommands({"pull", "pull jim hoe"});
    expected_sub = {"pull", "pull jim hoe", "", "push"};
    cli.init();
    EXPECT_EQ(cli.getSubcommands(true), expected_sub);
    cli.addFlags("push", {"-f"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "push");

    cli.setArguments({"MyProgram", "pull", "jim", "hoe", "-g", "value"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "pull jim hoe");
    cli.addFlags("pull jim hoe", {"-f"});
    EXPECT_THROW(cli.init(), CLIException);
    EXPECT_EQ(cli.isFlagValid("-f"), true);
    EXPECT_EQ(cli.isFlagActive("-f"), false);
}

TEST(addFlags, multiple_single_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hmv", "--help", "stuff"});
    cli.addSubcommands({});
    cli.addFlags({"-h", "--help", "-m", "-v"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("-h"), 1);
    EXPECT_EQ(cli.getFlagPosition("-m"), 1);
    EXPECT_EQ(cli.getFlagPosition("-v"), 1);
    EXPECT_EQ(cli.getFlagPosition("--help"), 2);

    cli.clear();
    cli.setArguments({"MyProgram", "-hmv", "-gc", "--hello"});
    cli.addFlags({"-h", "-m", "-v", "-g", "-c", "--hello"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("-h"), 1);
    EXPECT_EQ(cli.getFlagPosition("-m"), 1);
    EXPECT_EQ(cli.getFlagPosition("-v"), 1);
    EXPECT_EQ(cli.getFlagPosition("-g"), 2);
    EXPECT_EQ(cli.getFlagPosition("-c"), 2);
    EXPECT_EQ(cli.getFlagPosition("--hello"), 3);

    cli.clear();
    cli.setArguments({"MyProgram", "-hmvx", "-gc", "--hello"});
    cli.addFlags({"-h", "-m", "-v", "-g", "-c", "--hello"});
    EXPECT_THROW(cli.init(), CLIException);
}

TEST(addFlags, duplicate_multiple_single_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-uvv", "hello"});
    cli.addFlags({"-h", "-u", "-v"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("-h"), -1);
    EXPECT_EQ(cli.getFlagPosition("-u"), 1);
    EXPECT_EQ(cli.getFlagPosition("-v"), 1);

    cli.setArguments({"MyProgram", "-uvv", "-v", "hello"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("-v"), 2);
}

TEST(addFlags, substring_in_another_flag)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hmv", "--helpme", "stuff"});
    cli.addSubcommands({});
    cli.addFlags({"-h", "--help", "-m", "-v", "--helpme"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("-h"), 1);
    EXPECT_EQ(cli.getFlagPosition("-m"), 1);
    EXPECT_EQ(cli.getFlagPosition("-v"), 1);
    EXPECT_EQ(cli.getFlagPosition("--help"), -1);
    EXPECT_EQ(cli.getFlagPosition("--helpme"), 2);

    cli.setArguments({"MyProgram", "--help", "--helpme", "stuff"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("--help"), 1);
    EXPECT_EQ(cli.getFlagPosition("--helpme"), 2);

    cli.setArguments({"MyProgram", "--helpme", "--help", "stuff"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("--help"), 2);
    EXPECT_EQ(cli.getFlagPosition("--helpme"), 1);
}

TEST(addFlags, duplicate_flags_initialization)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--help", "-h", "stuff"});
    cli.addFlags({"-h", "--help", "-h", "--help"});
    cli.init();
    EXPECT_EQ(cli.areFlagsActive({"-h", "--help"}), true);
}

TEST(addFlags, duplicate_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--help", "--help"});
    cli.addFlags({"--help"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("--help"), 2);

    cli.clear();
    cli.setArguments({"MyProgram", "--flag=value", "--flag"});
    cli.addFlags({"--flag"});
    cli.init();
    EXPECT_EQ(cli.getFlagPosition("--flag"), 2);
    EXPECT_EQ(cli.getAllValuesOf("--flag"), vector<string>());
}

TEST(addFlags, flags_with_invalid_prefix)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--help", "-h", "stuff"});
    cli.init();
    EXPECT_THROW(cli.addFlags({"--help", "/h"}), CLIException);
    EXPECT_THROW(cli.addFlags({"---help"}), CLIException);
    EXPECT_THROW(cli.addFlags({"money"}), CLIException);
}

TEST(addFlags, multiple_single_flag_initialization)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hmv"});
    cli.init();
    EXPECT_THROW(cli.addFlags({"-hmv"}), CLIException);
    EXPECT_THROW(cli.addFlags({"-help"}), CLIException);
}

TEST(addFlags, invalid_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hel-lo", "stuff"});
    cli.addFlags({"-h", "-e", "-o"});
    EXPECT_THROW(cli.init(), CLIException);

    cli.clear();
    cli.addFlags({"-h", "-e", "-o"});
    cli.setArguments({"MyProgram", "-hel-lo", "stuff"});
    EXPECT_THROW(cli.init(), CLIException);
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

TEST(addGlobalFlags, general)
{
    unordered_set<string> expected_flags = {"-h", "--help"};
    CLI cli;
    cli.setArguments({"MyProgram", "-h", "--help", "-m", "-vi"});
    cli.addGlobalFlags({"-h", "--help"});
    EXPECT_EQ(cli.getFlags(), expected_flags);
    
    cli.addSubcommands({"init", "add"});
    cli.addGlobalFlags({"-h", "--help"});
    cli.addFlags("add", {"-v", "--verbose"});
    expected_flags = {"-h", "--help"};
    EXPECT_EQ(cli.getFlags(), expected_flags);
    EXPECT_EQ(cli.getFlags("init"), expected_flags);
    expected_flags = {"-h", "--help", "-v", "--verbose"};
    EXPECT_EQ(cli.getFlags("add"), expected_flags);
}

TEST(addGlobalFlags, excludes)
{
    unordered_set<string> expected_flags = {"-h", "--help"};
    
    CLI cli;
    cli.setArguments({"MyProgram", "-h", "--help", "-v", "--verbose"});
    cli.addSubcommands({"init", "add"});
    cli.addGlobalFlags({"-h", "--help"}, {"add"});
    EXPECT_EQ(cli.getFlags(), expected_flags);
    EXPECT_EQ(cli.getFlags("init"), expected_flags);
    expected_flags.clear();
    EXPECT_EQ(cli.getFlags("add"), expected_flags);

    cli.addGlobalFlags({"-v", "--verbose"}, {"", "init"});
    expected_flags = {"-v", "--verbose"};
    EXPECT_EQ(cli.getFlags("add"), expected_flags);
}

TEST(removeGlobalFlags, general)
{
    unordered_set<string> expected_flags = {"-h", "--help", "-v", "--verbose"};

    CLI cli;
    cli.setArguments({"MyProgram", "init", "-h", "--help", "-v", "--verbose"});
    cli.addSubcommands({"init", "add", "remote add"});
    cli.addGlobalFlags({"-h", "--help", "-v", "--verbose"});
    EXPECT_EQ(cli.getFlags(), expected_flags);
    cli.removeGlobalFlags({"-h", "--help", "-v", "--verbose", "4"});
    expected_flags.clear();
    EXPECT_EQ(cli.getFlags(), expected_flags);
}

TEST(removeGlobalFlags, excludes)
{
    unordered_set<string> expected_flags = {"-h", "--help", "-v", "--verbose"};

    CLI cli;
    cli.setArguments({"MyProgram", "init", "-h", "--help", "-v", "--verbose"});
    cli.addSubcommands({"init", "add", "remote add"});
    cli.addGlobalFlags({"-h", "--help", "-v", "--verbose"});
    EXPECT_EQ(cli.getFlags(), expected_flags);
    cli.removeGlobalFlags({"-h", "--help", "-v", "--verbose", "4"}, {"init", "remote add", "rr"});
    EXPECT_EQ(cli.getFlags("remote add"), expected_flags);
    EXPECT_EQ(cli.getFlags("init"), expected_flags);
    expected_flags.clear();
    EXPECT_EQ(cli.getFlags(), expected_flags);
    EXPECT_EQ(cli.getFlags("add"), expected_flags);
}

TEST(removeFlags, general)
{
    unordered_set<string> expected_flags = {"-h", "--help", "-v", "--verbose"};

    CLI cli;
    cli.setArguments({"MyProgram", "init", "-h", "--help"});
    cli.addSubcommands({"init", "add", "remote add"});
    cli.addGlobalFlags({"-h", "--help", "-v", "--verbose"});
    EXPECT_EQ(cli.getFlags(), expected_flags);
    cli.removeFlags({"-h", "-v", "-k"});
    expected_flags = {"--help", "--verbose"};
    EXPECT_EQ(cli.getFlags(), expected_flags);
}

TEST(initialization, order)
{
    // initialization order test
    vector<string> expected_args = {"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"};
    unordered_set<string> expected_subs = {"remote", "remote add", ""};

    // args -> subcommands -> flags
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"});
    cli.addSubcommands({"remote", "remote add"});
    cli.addFlags("remote add", {"-h", "--output", "-o"});
    cli.init();
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add");
    EXPECT_EQ(cli.getSubcommands(true), expected_subs);
    EXPECT_EQ(cli.areFlagsActive({"-h", "--output"}), true);

    // subcommands -> args -> flags
    cli.clear();
    cli.addSubcommands({"remote", "remote add"});
    cli.setArguments({"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"});
    cli.addFlags("remote add", {"-h", "--output", "-o"});
    cli.init();
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add");
    EXPECT_EQ(cli.getSubcommands(true), expected_subs);
    EXPECT_EQ(cli.areFlagsActive({"-h", "--output"}), true);

    // flags -> subcommands -> args
    cli.clear();
    EXPECT_THROW(cli.addFlags("remote add", {"-h", "--output", "-o"}), CLIException);
    cli.addSubcommands({"remote", "remote add"});
    cli.setArguments({"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"});
    cli.init();
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add");
    EXPECT_EQ(cli.getSubcommands(true), expected_subs);
    EXPECT_THROW(cli.areFlagsActive({"-h", "--output"}), CLIException);

    // flags -> args -> subcommands
    cli.clear();
    EXPECT_THROW(cli.addFlags("remote add", {"-h", "--output", "-o"}), CLIException);
    cli.setArguments({"MyProgram", "remote", "add", "-h", "val1", "--output", "val2"});
    cli.addSubcommands({"remote", "remote add"});
    cli.init();
    EXPECT_EQ(cli.getArguments(), expected_args);
    EXPECT_EQ(cli.getActiveSubcommand(), "remote add");
    EXPECT_EQ(cli.getSubcommands(true), expected_subs);
    EXPECT_THROW(cli.areFlagsActive({"-h", "--output"}), CLIException);
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
    cli.init();

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

TEST(getActiveFlagAmong, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-h", "hi", "-v"});
    cli.addFlags({"-h", "--help", "-v", "--verbose"});
    cli.init();
    EXPECT_EQ(cli.getActiveFlagAmong({"-h", "--help"}), "-h");
    EXPECT_EQ(cli.getActiveFlagAmong({"-v", "--verbose"}), "-v");
}

TEST(getAllActiveFlagsAmong, general)
{
    vector<string> expected_flags = {"-h", "--help", "-v"};

    CLI cli;
    cli.setArguments({"MyProgram", "-h", "--help", "-v", "hi"});
    cli.addFlags({"-h", "--help", "-v", "--verbose"});
    cli.init();
    EXPECT_EQ(cli.getAllActiveFlagsAmong({"-h", "--help", "-v"}), expected_flags);
    EXPECT_EQ(cli.getAllActiveFlagsAmong({"-h", "--help", "-v", "--verbose"}), expected_flags);
}

TEST(getAllActiveFlagsAmong, multiple_flags)
{
    vector<string> expected_flags = {"-h", "--help"};
    
    CLI cli;
    cli.setArguments({"MyProgram", "-h", "-h", "--help"});
    cli.addFlags({"-h", "--help", "-v", "--verbose"});
    cli.init();
    EXPECT_EQ(cli.getAllActiveFlagsAmong({"-h", "--help", "-v", "--verbose"}), expected_flags);
}

TEST(getAnyValue, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "init", "cpp-app", "--path", "value1", "-f", "trueval"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"--path", "-f"});
    cli.init();
    EXPECT_EQ(cli.getAnyValue({"--path"}), "cpp-app");
    EXPECT_EQ(cli.getAnyValue(2, {"--path"}), "trueval");

    cli.clear();
    cli.setArguments({"MyProgram", "init", "--path", "my/path", "cpp-app"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"--path", "-f"});
    cli.init();
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
    cli.init();
    EXPECT_EQ(cli.getAnyValue({"-h"}), "value1");
    EXPECT_EQ(cli.getAnyValue({"-i"}), "value1");
    EXPECT_EQ(cli.getAnyValue({"-v"}), "value1");
    EXPECT_EQ(cli.getAnyValue({"-h", "-i", "-v"}), "value2");
}

TEST(getAnyValue, equal_sign)
{
    CLI cli;
    cli.setArguments({"MyProgram", "value1", "-hiv=value2", "--verbose=value3"});
    cli.addFlags({"-h", "-i", "-v", "--verbose"});
    cli.init();

    EXPECT_EQ(cli.getAnyValue(), "value1");
    EXPECT_EQ(cli.getAnyValue(2), "value2");
    EXPECT_EQ(cli.getAnyValue(2), "value2");
    EXPECT_EQ(cli.getAnyValue(2), "value2");
    EXPECT_EQ(cli.getAnyValue(2, {"-h", "-i"}), "value2");
    EXPECT_EQ(cli.getAnyValue(2, {"-h", "-i", "-v"}), "value3");
    EXPECT_EQ(cli.getAnyValue(3), "value3");
}

TEST(getAllValues, general)
{
    vector<string> expected_vals = {"cpp-app", "value1", "trueval"};

    CLI cli;
    cli.setArguments({"MyProgram", "init", "cpp-app", "--path", "value1", "-f", "trueval"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"--path", "-f"});
    cli.init();
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
    cli.init();
    EXPECT_EQ(cli.getAllValues({"-h"}), expected_vals);
    EXPECT_EQ(cli.getAllValues({"-i"}), expected_vals);
    EXPECT_EQ(cli.getAllValues({"-v"}), expected_vals);
    expected_vals = {"value2"};
    EXPECT_EQ(cli.getAllValues({"-h", "-i", "-v"}), expected_vals);
}

TEST(getAllValues, equal_sign)
{
    vector<string> expected_vals = {"value1", "value2", "value3", "value4"};

    CLI cli;
    cli.setArguments({"MyProgram", "init", "value1", "-hiv=value2", "value3", "-q", "value4"});
    cli.addSubcommands({"init"});
    cli.addFlags("init", {"-h", "-i", "-v", "-q"});
    cli.init();

    EXPECT_EQ(cli.getAllValues(), expected_vals);
    expected_vals = {"value1", "value2"};
    EXPECT_EQ(cli.getAllValues(2), expected_vals);
    expected_vals = {"value1", "value4"};
    EXPECT_EQ(cli.getAllValues({"-h", "-i", "-v"}), expected_vals);
    expected_vals = {"value1", "value2", "value3", "value4"};
    EXPECT_EQ(cli.getAllValues({"-h", "-i"}), expected_vals);
}

TEST(getValueOf, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--flag=value"});
    cli.addFlags({"--flag"});
    cli.init();
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
    cli.init();
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
    cli.init();
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
    cli.init();
    EXPECT_EQ(cli.getValueOf("--flag"), "value2");
}

TEST(getValueOf, no_value)
{
    CLI cli;
    cli.setArguments({"MyProgram", "-hmv"});
    cli.addFlags({"-h", "-m", "-v"});
    cli.init();
    EXPECT_EQ(cli.getValueOf("-h"), "");
    EXPECT_EQ(cli.getValueOf("-m"), "");
    EXPECT_EQ(cli.getValueOf("-v"), "");
}

TEST(getAllValuesOf, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--flag=value"});
    cli.addFlags({"--flag"});
    cli.init();
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
    cli.init();
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
    cli.init();
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
    vector<string> expected_values;

    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "val1", "val2", "--flag=fval1", "fval2", "-h", "hval1", "hval2"});
    cli.addSubcommands({"remote add", "remote"});
    cli.addFlags("remote add", {"--flag", "-h", "--help"});
    cli.init();

    expected_values = {"val1", "val2"};
    EXPECT_EQ(cli.getAllValuesOf(), expected_values);
    expected_values = {"fval1", "fval2"};
    EXPECT_EQ(cli.getAllValuesOf("--flag"), expected_values);
    expected_values = {"hval1", "hval2"};
    EXPECT_EQ(cli.getAllValuesOf("-h"), expected_values);

    cli.clear();
    cli.setArguments({"MyProgram", "val1", "val2", "--flag=fval1", "fval2", "-h", "hval1", "hval2"});
    cli.addFlags({"--flag", "-h", "--help"});
    cli.init();
    expected_values = {"val1", "val2"};
    EXPECT_EQ(cli.getAllValuesOf(), expected_values);
    expected_values = {"fval1", "fval2"};
    EXPECT_EQ(cli.getAllValuesOf("--flag"), expected_values);
    expected_values = {"hval1", "hval2"};
    EXPECT_EQ(cli.getAllValuesOf("-h"), expected_values);

    cli.setArguments({"MyProgram", "--flag=fval1", "fval2", "-h", "hval1", "hval2"});
    cli.init();

    expected_values = {};
    EXPECT_EQ(cli.getAllValuesOf(), expected_values);
    expected_values = {"fval1", "fval2"};
    EXPECT_EQ(cli.getAllValuesOf("--flag"), expected_values);
    expected_values = {"hval1", "hval2"};
    EXPECT_EQ(cli.getAllValuesOf("-h"), expected_values);
}

TEST(checkers, general)
{
    CLI cli;
    cli.setArguments({"MyProgram", "home", "-g"});
    cli.init();
    EXPECT_EQ(cli.isFlagValid("-g"), false);
    EXPECT_THROW(cli.isFlagActive("-g"), CLIException);
    EXPECT_THROW(cli.getFlagPosition("-g"), CLIException);

    cli.clearFlags();
    cli.addFlags({"-g"});
    cli.init();
    EXPECT_EQ(cli.isSubcommandValid("home"), false);
    EXPECT_EQ(cli.isFlagValid("-g"), true);
    EXPECT_EQ(cli.isFlagActive("-g"), true);
    EXPECT_EQ(cli.getFlagPosition("-g"), 2);

    cli.addSubcommands({"home", "init"}); // will reset valid flags
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "home");
    EXPECT_EQ(cli.isFlagValid("-g"), false);
    EXPECT_THROW(cli.isFlagActive("-g"), CLIException); // because "-g" is not a valid flag of "home"
    EXPECT_THROW(cli.getFlagPosition("-g"), CLIException);

    EXPECT_THROW(cli.addFlags("test", {"--help"}), CLIException);
    cli.addFlags("home", {"-g", "--help"});
    cli.setArguments({"MyProgram", "home", "-g", "--help"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "home");
    EXPECT_EQ(cli.isFlagValid("-g"), true);
    EXPECT_EQ(cli.isFlagValid("--help"), true);
    EXPECT_EQ(cli.isFlagActive("-g"), true);
    EXPECT_EQ(cli.isFlagActive("--help"), true);
    EXPECT_EQ(cli.getFlagPosition("-g"), 2);
    EXPECT_EQ(cli.getFlagPosition("--help"), 3);

    cli.setArguments({"MyProgram", "init", "-g", "this"});
    cli.init();
    EXPECT_EQ(cli.getActiveSubcommand(), "init");
    EXPECT_EQ(cli.isFlagValid("-g"), false);
    EXPECT_THROW(cli.isFlagActive("-g"), CLIException);
    EXPECT_THROW(cli.getFlagPosition("-g"), CLIException);

    cli.addFlags("init", {"-g"});
    cli.init();
    EXPECT_EQ(cli.isFlagValid("-g"), true);
    EXPECT_EQ(cli.isFlagActive("-g"), true);
    EXPECT_EQ(cli.getFlagPosition("-g"), 2);
}

TEST(isFlagActive, multiple_flags)
{
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "-us", "hello", "--substring", "hi"});
    cli.addFlags({"-u", "--unscramble", "-s", "--substring"});
    cli.init();
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
    EXPECT_EQ(cli.getSubcommands(true), expected_subs);
    EXPECT_EQ(cli.getActiveSubcommand(), expected_active_sub);
}