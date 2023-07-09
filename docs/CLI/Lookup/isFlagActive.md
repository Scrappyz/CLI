## CLI::isFlagActive
Defined in header `cli.hpp`

| |
| --- |
| bool isFlagActive(const std::string& flag) |
| bool isFlagActive(const std::initializer_list\<std::string>& flag) |
| bool isFlagActive(const std::vector\<std::string>& flag) |

Checks if a given flag or atleast one flag in a given list of flags is present in the argument list.

## Parameters
`flag` - the flag(s) to check

## Return Value
Returns `true` if the given flag or atleast one flag in a given list of flags is present in the argument list, `false` otherwise.

## Error
Throws a `CLIException` error if:
- one of the flags is invalid

## Notes
- Providing a list of flags such as `isActiveFlag({"-h", "--help"})` is basically the same as `isActiveFlag("-h") || isActiveFlag("--help")`

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
            if(cli.isFlagActive({"-h", "--help"})) {
                std::cout << "help flag is active in remote add" << std::endl;
            } else if(cli.isFlagActive("--tags")) {
                std::cout << "tag flag is active in remote add" << std::endl;
            } else if(cli.isFlagActive("--verbose")) {
                std::cout << "verbose flag is active in remote add" << std::endl;
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
CLI.exe status --verbose 
```

Output:
```
verbose flag is active in status
```