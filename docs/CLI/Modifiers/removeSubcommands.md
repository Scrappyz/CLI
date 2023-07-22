## CLI::removeSubcommands
Defined in header `cli.hpp`

| |
| --- |
| void removeSubcommands(const std::vector\<std::string>& subcmds) |

Removes the given subcommands.

## Parameters
`subcmd` - the subcommands to remove

## Example
```
#include <iostream>
#include "cli.hpp"

void print(const std::unordered_set<std::string>& s)
{
    std::cout << "Subcommands:" << std::endl;
    for(const auto& i : s) {
        std::cout << "  " << i << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.addSubcommands({"init", "add", "remote add"});
        print(cli.getSubcommands());
        cli.removeSubcommands({"init", "add"});
        print(cli.getSubcommands());
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

Output:
```
Subcommands:
  init
  add
  remote add

Subcommands:
  remote add
```