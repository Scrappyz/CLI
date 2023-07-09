## CLI::areFlagsActive
Defined in header `cli.hpp`

| |
| --- |
| bool areFlagsActive(const std::vector\<std::string>& flags) |

Checks if all the flags in the given list of flags are present in the argument list.

## Parameters
`flags` - the list of valid flags

## Return Value
Returns `true` if all the flags in the given list are active, `false` otherwise.

## Error
Throws a `CLIException` error if:
- one of the flags is invalid

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

        if(cli.noActiveSubcommand()) {
            if(cli.isFlagActive({"-h", "--help"})) {
                std::cout << "help flag is active" << std::endl;
            }
        } else if(cli.isActiveSubcommand("add")) {
            if(cli.isFlagActive("--path")) {
                std::cout << "path flag is active in add" << std::endl;
            }
        } else if(cli.isActiveSubcommand("status")) {
            if(cli.isFlagActive({"-v", "--verbose"})) {
                std::cout << "verbose flag is active in status" << std::endl;
            }
        } else if(cli.isActiveSubcommand("remote add")) {
            if(cli.areFlagsActive({"--tags", "-h", "--help", "--verbose"})) {
                std::cout << "all flags are active on remote add" << std::endl;
            }
        }
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe remote add -h --help --tags --verbose
```

Output:
```
all flags are active on remote add
```