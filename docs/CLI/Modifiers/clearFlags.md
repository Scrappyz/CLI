## CLI::clearFlags
Defined in header `cli.hpp`

| | 
| --- |
| void clearFlags(const std::string& subcmd = "") |

Clears all flags from the given subcommand.

## Parameters
`subcmd` - the subcommand to clear the flags from

## Error
Throws a `CLIException` error if the given subcommand is invalid

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
        cli.addSubcommands({"init", "add", "remote add"});
        cli.addGlobalFlags({"-h", "-v", "--help", "--verbose"});
        print("init", cli.getFlags("init"));
        cli.clearFlags("init");
        print("init", cli.getFlags("init"));
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Output:
```
init: "-h" "-v" "--help" "--verbose" 
init:
```