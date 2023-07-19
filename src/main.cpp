#include <iostream>
#include "cli.hpp"

void printArgs(const std::vector<std::string>& args)
{
    for(int i = 0; i < args.size(); i++) {
        std::cout << args[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    cli.setArguments({"MyProgram", "--help", "-h", "stuff"});
    try {
        // cli.setValidSubcommands({"pull", "pull jim hoe"});
        // cli.setValidFlags("push", {"-f"});
        //cli.setValidFlags({"-"});
        
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}