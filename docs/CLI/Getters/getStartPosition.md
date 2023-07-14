## CLI::getStartPosition
Defined in header `cli.hpp`

| |
| --- |
| int getStartPosition() |

Returns the index after the subcommand which can be used when searching through the argument list.

## Return Value
Returns the index after the subcommand.

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

        std::cout << cli.getStartPosition() << std::endl;
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe remote add -h
```

Output:
```
3
```