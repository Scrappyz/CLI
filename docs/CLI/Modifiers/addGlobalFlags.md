## CLI::addGlobalFlags
Defined in header `cli.hpp`

| |
| --- |
| void addGlobalFlags(const std::vector\<std::string>& valid_flags, const std::unordered_set\<std::string>& excludes = {}) |

Adds the given flags and makes them available for all subcommands.

## Parameters
`valid_flags` - the flags to make available \
`excludes` - excluded subcommands

## Error
Throws a `CLIException` error if:
- one of the given flags is invalid

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
init: "-h" "--help" "-v" "--verbose" 
add: "-h" "--help" "-v" "--verbose" 
```