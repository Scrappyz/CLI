## CLI::getValidFlags
Defined in header `cli.hpp`

| |
| --- |
| std::unordered_set\<std::string> getValidFlags(std::string subcmd = "") |

## Parameters
`subcmd` - the subcommand to get the valid flags of

## Return Value
Returns all the valid flags of a given valid subcommand.

## Error
Throws a `CLIException` error if the given subcommand is not valid.

## Example
```
#include <iostream>
#include "cli.hpp"

void printValidFlags(const std::unordered_set<std::string>& flags)
{
    for(const auto& i : flags) {
        std::cout << "\"" << i << "\" ";
    }
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "-v"});

        printValidFlags(cli.getValidFlags());
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe remote add --tags mytag -h
```

Output:
```
"--tags" "-h" "--help" "-v" 
```