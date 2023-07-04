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
    cli.setArguments({"MyProgram"});
    cout << cli.getProgramName() << endl;
    
    return 0;
}