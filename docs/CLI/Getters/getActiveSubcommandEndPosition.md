## CLI::getActiveSubcommandEndPosition
Defined in header `cli.hpp`

| |
| --- |
| int getActiveSubcommandEndPosition() |

## Return Value
Returns the index of the last word of the active subcommand in the argument list.

## Example
```
#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags"});

        std::cout << "Active Subcommand: " << cli.getActiveSubcommand() << std::endl;
        std::cout << "Active Subcommand End Position: " << cli.getActiveSubcommandEndPosition() << std::endl;
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe remote add --tags mytag
```

Output:
```
Active Subcommand: remote add
Active Subcommand End Position: 2
```