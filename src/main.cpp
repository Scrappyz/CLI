#include <iostream>
#include "cli.hpp"

void print(const std::unordered_set<std::string>& s)
{
    for(const auto& i : s) {
        std::cout << i << std::endl;
    }
}

int main(int argc, char* argv[])
{
    // CLI cli(argc, argv);
    // cli.setArguments({"MyProgram", "value1", "-vh=value2", "--answer", "value3"});
    // try {
    //     // cli.addFlags({"-v", "--answer", "--question", "-h"});
    //     // cli.init();

    //     // std::cout << cli.getAnyValue(1) << std::endl;
    //     // std::cout << cli.getAnyValue(2) << std::endl;
    //     // std::cout << cli.getAnyValue(3) << std::endl;
    //     throw CLIException(__func__, "My Exception");
    // } catch(const CLIException& e) {
    //     std::cout << e.what() << std::endl;
    //     return 1;
    // }

    std::unordered_set<std::string> s = {"init", "bang"};
    s.erase("bang");
    print(s);
    return 0;
}