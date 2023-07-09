## CLI::isActiveSubcommand
Defined in header `cli.hpp`

| |
| --- |
| bool isActiveSubcommand(const std::string& subcmd = "") |

Checks if the given subcommand is the active subcommand.

## Parameters
`subcmd` - the subcommand to check

## Return Value
Returns `true` if the given subcommand is the active subcommand, `false` otherwise.

## Example
```
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

        std::cout << cli.isActiveSubcommand() << std::endl; // check empty subcommand
        std::cout << cli.isActiveSubcommand("add") << std::endl;
        std::cout << cli.isActiveSubcommand("status") << std::endl;
        std::cout << cli.isActiveSubcommand("remote add") << std::endl;
        std::cout << cli.isActiveSubcommand("some subcommand") << std::endl;
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe status
```

Output:
```
0
0
1
0
0
```