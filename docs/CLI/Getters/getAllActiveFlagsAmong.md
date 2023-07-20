## CLI::getAllActiveFlagsAmong
Defined in header `cli.hpp`

| |
| --- |
| std::vector\<std::string> getAllActiveFlagsAmong(const std::vector\<std::string>& flags) |

## Parameters
`flags` - a list of valid flags

## Return Value
Returns all the flags in the given list that are present in the argument list.

## Error
Throws a `CLIException` error if there is an invalid flag in the given list.

## Example
```
#include <iostream>
#include "cli.hpp"

void print(const std::vector<std::string>& v)
{
    for(const auto& i : v) {
        std::cout << "\"" << i << "\" ";
    }
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "-v"});

        print(cli.getAllActiveFlagsAmong({"--tags", "-h", "--help", "-v"}));
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
"--tags" "-h"
```