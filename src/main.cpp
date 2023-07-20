#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    cli.setArguments({"MyProgram", "value1", "-vh=value2", "--answer", "value3"});
    try {
        cli.addFlags({"-v", "--answer", "--question", "-h"});
        cli.init();

        std::cout << cli.getAnyValue(1) << std::endl;
        std::cout << cli.getAnyValue(2) << std::endl;
        std::cout << cli.getAnyValue(3) << std::endl;
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}