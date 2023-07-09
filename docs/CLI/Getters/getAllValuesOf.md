## CLI::getAllValuesOf
Defiend in header `cli.hpp`

| |
| --- |
| std::vector\<std::string> getAllValuesOf(int limit = -1) |
| std::vector\<std::string> getAllValuesOf(const std::initializer_list\<std::string>& flag, int limit = -1) |
| std::vector\<std::string> getAllValuesOf(const std::vector\<std::string>& flag, int limit = -1) |
| std::vector\<std::string> getAllValuesOf(const std::string& flag, int limit = -1) |

## Parameters
`flag` - the active flag to get the values of \
`limit` - the maximum number of values to be returned

## Return Value
Returns all the values of the given active flag.

## Error
Throws a `CLIException` error if the given flag is not present in the argument list.

## Notes
- Supports the `--flag=value` format. In this format, `--flag=value` is the same as `--flag value`.
- If no flag is provided, it will return the value next to the subcommand.

## Example
```
#include <iostream>
#include "cli.hpp"

void printValues(const std::vector<std::string>& v)
{
    for(const auto& i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "--verbose"});

        if(cli.isFlagActive("--tags")) { // check if "--tags" is present in the argument list
            std::cout << "tags: "; 
            printValues(cli.getAllValuesOf("--tags", 2)); // limit to 2
        }

        if(cli.isFlagActive("--verbose")) {
            std::cout << "verbose: ";
            printValues(cli.getAllValuesOf("--verbose"));
        }

        std::cout << "Program value: ";
        printValues(cli.getAllValuesOf(1)); // limit to 1
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe remote add val1 val2 --tags tag1 tag2 --verbose=true
```

Output:
```
tags: tag1 tag2 
verbose: true
Program value: val1
```