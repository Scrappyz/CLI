#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags({"-h", "--help"});
        cli.setValidFlags("add", {"--path"});
        cli.setValidFlags("status", {"-v", "--verbose"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "--verbose"});

        std::cout << cli.isValidFlag("", "-h") << std::endl;
        std::cout << cli.isValidFlag("add", "--path") << std::endl;
        std::cout << cli.isValidFlag("remote add", "--tags") << std::endl;
        std::cout << cli.isValidFlag("status", "--tags") << std::endl;
        std::cout << cli.isValidFlag("some subcommand", "-h") << std::endl;
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}