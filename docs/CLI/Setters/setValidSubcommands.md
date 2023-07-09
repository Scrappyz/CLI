## CLI::setValidSubcommands
Defined in header `cli.hpp`

| |
| --- |
| void setValidSubcommands(const std::vector\<std::string>& valid_subs) |

Initializes the subcommands the `CLI` object can take.

## Parameters
`valid_subs` - the list of valid subcommands

## Example
```
#include <iostream>
#include "cli.hpp"

void printSubcommands(const std::unordered_set<std::string>& subs)
{
    for(const auto& i : subs) {
        std::cout << "\"" << i << "\" ";
    }
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags"});

        printSubcommands(cli.getValidSubcommands());
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe remote add --tags mytag
```

Output:
```
"" "add" "status" "remote add" 
```