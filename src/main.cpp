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
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags({"-h", "--help"});
        cli.setValidFlags("add", {"--path"});
        cli.setValidFlags("status", {"-v", "--verbose"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "--verbose"});

        printArgs(cli.getArguments());
        printArgs(cli.getArguments(2));
        printArgs(cli.getArguments(1, 3));
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}