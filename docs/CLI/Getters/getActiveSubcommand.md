## CLI::getActiveSubcommand

| |
| --- |
| std::string getActiveSubcommand() |

## Return Value
Returns the current active subcommand.

## Notes
- If no subcommand is present in the argument list, it will return the empty subcommand which is denoted by the empty string `""`.

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
```