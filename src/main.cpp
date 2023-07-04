#include <iostream>
#include "cli.hpp"

using namespace std;

string check()
{
    return string("check()");
}

string check(bool error)
{
    return string("check(bool error)");
}

string check(const char* flag)
{
    return string("check(const char* flag)");
}

string check(const string& flag)
{
    return string("check(const string& flag)");
}

string check(const string& flag, bool error)
{
    return string("check(const string& flag, bool error)");
}

int main(int argc, char** argv)
{
    CLI cli;
    cli.setArguments({"MyProgram", "remote", "add", "val1", "val2", "--flag", "fval1", "fval2", "-h", "hval1", "hval2", "hval3",
    "-m=mval1", "mval2", "mval3", "-f", "ffval1", "ffval2"});
    cli.setValidSubcommands({"remote add"});
    cli.setValidFlags("remote add", {"--flag", "-h", "-m", "-f"});

    cout << cli.getValueOf() << endl;
    cout << cli.getValueOf("--flag") << endl;
    cout << cli.getValueOf({"--flag"}) << endl;
    
    return 0;
}