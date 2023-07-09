## CLI::noActiveSubcommand
Defined in header `cli.hpp`

| |
| --- |
| bool noActiveSubcommand() |

Checks if there is no active subcommand.

## Return Value
Returns `true` if there is no active subcommand, `false` otherwise.

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

        std::cout << cli.noActiveSubcommand() << std::endl;
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe not a subcommand
```

Output:
```
1
```