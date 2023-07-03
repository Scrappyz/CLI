#include <iostream>
#include "cli.hpp"

using namespace std;

int main(int argc, char** argv)
{
    CLI cli;
    cli.setArguments({"MyProgram", "--help", "--help"});
    cli.setValidFlags({"--help"});
    cli.setValidFlags({"--flag"});
    
    return 0;
}