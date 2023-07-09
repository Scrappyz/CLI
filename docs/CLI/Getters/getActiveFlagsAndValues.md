## CLI::getActiveFlagsAndValues
Defined in header `cli.hpp`

| | 
| --- |
| std::unordered_map\<std::string, std::vector\<std::string>> getActiveFlagsAndValues() |

## Return Value
Returns all the active flags and their values.

## Example
```
#include <iostream>
#include "cli.hpp"

void print(const std::unordered_map<std::string, std::vector<std::string>>& flags)
{
    for(const auto& i : flags) {
        std::cout << i.first << ": ";
        for(const auto& j : i.second) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "--verbose"});

        print(cli.getActiveFlagsAndValues());
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

Ouptut:
```
--tags: tag1 tag2 
--verbose: true
: val1 val2
```