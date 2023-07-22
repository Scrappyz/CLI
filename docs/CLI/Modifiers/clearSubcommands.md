## CLI::clearSubcommands
Defined in header `cli.hpp`

| |
| --- |
| void clearSubcommands() |

Clears all subcommands

## Example
```
#include <iostream>
#include "cli.hpp"

void print(const std::unordered_set<std::string>& s)
{
    std::cout << "Subcommands:" << std::endl;
    for(const auto& i : s) {
        if(i.empty()) {
            continue;
        }
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
        cli.clearSubcommands();
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

```