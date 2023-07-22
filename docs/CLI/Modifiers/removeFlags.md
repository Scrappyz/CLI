## CLI::removeFlags
Defined in header `cli.hpp`

| |
| --- |
| void removeFlags(const std::vector\<std::string>& flags) |
| void removeFlags(const std::string& subcmd, const std::vector\<std::string>& flags) |

Removes the flags from the given subcommand.

## Parameters
`subcmd` - the subcommand to remove the flags from \
`flags` - the flags to remove

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
        cli.removeFlags("init", {"-h", "-v"});
        cli.removeFlags("add", {"--help", "--verbose"});

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
: "-h" "--help" "-v" "--verbose" 
init: "--help" "--verbose"
add: "-h" "-v"
```