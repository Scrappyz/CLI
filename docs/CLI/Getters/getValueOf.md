## CLI::getValueOf
Defined in header `cli.hpp`

| | 
| --- |
| std::string getValueOf(int occurance = 1) |
| std::string getValueOf(const std::initializer_list\<std::string>& flag, int occurance = 1) |
| std::string getValueOf(const std::vector\<std::string>& flag, int occurance = 1) |
| std::string getValueOf(const std::string& flag, int occurance = 1) |

## Parameters
`flag` - the flag to get the value of
`occurance` - which value to get from the flag

## Return Value
Returns the value of the given flag.

## Error
Throws a `CLIException` error if the given flag is not present in the argument list.

## Notes
- Supports the `--flag=value` format. In this format, `--flag=value` is the same as `--flag value`.
- If no flag is provided, it will return the value next to the subcommand.

## Example
```
#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "--verbose"});

        if(cli.isFlagActive("--tags")) { // check if "--tags" is present in the argument list
            std::cout << "tags: "; 
            std::cout << cli.getValueOf("--tags") << " ";
            std::cout << cli.getValueOf("--tags", 2) << " "; // get the 2nd value of "--tags"
            std::cout << cli.getValueOf("--tags", 3) << std::endl; // get the 3rd value of "--tags"
        }

        if(cli.isFlagActive("--verbose")) {
            std::cout << "verbose: " << cli.getValueOf("--verbose") << std::endl;
        }

        std::cout << "Program value: " << cli.getValueOf() << " ";
        std::cout << cli.getValueOf(2) << std::endl;
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
Program value: val1 val2
```