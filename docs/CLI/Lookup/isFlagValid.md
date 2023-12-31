## CLI::isFlagValid
Defined in header `cli.hpp`

| |
| --- |
| bool isFlagValid(const std::string& flag) |
| bool isFlagValid(const std::string& subcmd, const std::string& flag) |

Checks if the given flag is a valid flag of a given subcommand.

## Parameters
`subcmd` - the subcommand to check the given flag for validity \
`flag` - the flag to check

## Return Value
Returns `true` if the flag is valid, `false` otherwise.

## Error
Throws a `CLIException` error if:
- the given subcommand is not a valid subcommand

## Notes
- If no subcommand is provided, it will check if the flag is valid for the active subcommand

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

        std::cout << cli.isFlagValid("", "-h") << std::endl;
        std::cout << cli.isFlagValid("add", "--path") << std::endl;
        std::cout << cli.isFlagValid("remote add", "--tags") << std::endl;
        std::cout << cli.isFlagValid("status", "--tags") << std::endl;
        std::cout << cli.isFlagValid("some subcommand", "-h") << std::endl;
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
[Error][isFlagValid] "some subcommand" is not a valid subcommand
```