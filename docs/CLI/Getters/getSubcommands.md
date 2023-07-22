## CLI::getSubcommands
Defined in header `cli.hpp`

| |
| --- |
| std::unordered_set\<std::string> getSubcommands(bool include_empty = false) |

## Parameters
`include_empty` - include empty subcommand

## Return Value
Returns all the valid subcommands. Excludes the empty subcommand if `include_empty` is false.

## Notes
- The empty string that is returned along with the other valid subcommands is the empty subcommand. It is the subcommand that is active when there is no subcommand in the argument list.

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

        printSubcommands(cli.getSubcommands());
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