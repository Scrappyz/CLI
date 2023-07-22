## CLI::removeGlobalFlags
Defined in header `cli.hpp`

| |
| --- |
| void removeGlobalFlags(const std::vector\<std::string>& flags, const std::unordered_set\<std::string>& excludes = {}) |

Removes the given flags from all subcommands.

## Parameters
`flags` - the flags to remove
`excludes` - excluded subcommands

## Example
```
#include <iostream>
#include "cli.hpp"

void print(const std::string& subcmd, const std::unordered_set<std::string>& s)
{
    std::cout << subcmd << ": ";
    for(const auto& i : s) {
        std::cout << "\"" + i << "\" ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.addSubcommands({"init", "add"});
        cli.addGlobalFlags({"-h", "--help", "-v", "--verbose"});
        cli.removeGlobalFlags({"-h", "-v"});

        print("", cli.getFlags()); // empty subcommand
        print("init", cli.getFlags("init"));
        print("add", cli.getFlags("add"));
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Output:
```
: "--help" "--verbose" 
init: "--help" "--verbose"
add: "--help" "--verbose"
```