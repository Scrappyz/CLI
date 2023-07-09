## CLI::getActiveFlags
Defined in header `cli.hpp`

| |
| --- |
| std::unordered_set\<std::string> getActiveFlags() |

## Return Value
Returns all the flags that are present in the argument list.

## Example
```
#include <iostream>
#include "cli.hpp"

void printActiveFlags(const std::unordered_set<std::string>& flags)
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

        printActiveFlags(cli.getActiveFlags());
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