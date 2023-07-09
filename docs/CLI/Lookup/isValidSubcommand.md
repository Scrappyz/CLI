## CLI::isValidSubcommand
Defined in header `cli.hpp`

| |
| --- |
| bool isValidSubcommand(const std::string& subcmd) |

Checks if the given string is a valid subcommand.

## Parameters
`subcmd` - the subcommand to check

## Return Value
Returns `true` if the given string is a valid subcommand, `false` otherwise.

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

        std::cout << cli.isValidSubcommand("add") << std::endl;
        std::cout << cli.isValidSubcommand("status") << std::endl;
        std::cout << cli.isValidSubcommand("remote add") << std::endl;
        std::cout << cli.isValidSubcommand("some sub") << std::endl;
        std::cout << cli.isValidSubcommand("hello") << std::endl;
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Output:
```
1
1
1
0
0
```