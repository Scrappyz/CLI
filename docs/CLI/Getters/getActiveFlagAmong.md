## CLI::getActiveFlagAmong
Defined in header `cli.hpp`

| | 
| --- |
| std::string getActiveFlagAmong(const std::vector\<std::string>& flags) |

## Parameters
`flags` - a list of valid flags

## Return Value
Returns the first flag in the given list that is present in the argument list.

## Error
Throws a `CLIException` error if an invalid flag is found in the given list.

## Example
```
#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "-v"});

        std::cout << cli.getActiveFlagAmong({"-h", "--tags"}) << std::endl; // "-h" comes first
        std::cout << cli.getActiveFlagAmong({"--tags", "-h"}) << std::endl; // "--tags" comes first
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
-h
--tags
```