## CLI::getActiveFlagsAndPositions
Defined in header `cli.hpp`

| | 
| --- |
| std::unordered_map\<std::string, int> getActiveFlagsAndPositions() |

## Return Value
Returns all the flags that are present in the argument list as well as their positions.

## Example
```
#include <iostream>
#include "cli.hpp"

void printActiveFlagsAndPositions(const std::unordered_map<std::string, int>& flags)
{
    for(const auto& i : flags) {
        std::cout << "[\"" << i.first << "\", " << i.second << "]" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "-v"});

        printActiveFlagsAndPositions(cli.getActiveFlagsAndPositions());
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
["--tags", 3]
["-h", 5]
```