## CLI::addFlags
Defined in header `cli.hpp`

| |
| --- |
| void addFlags(const std::vector\<std::string>& valid_flags) |
| void addFlags(const std::string& subcmd, const std::vector\<std::string>& valid_flags) |

Initializes the flags a subcommand can take.

## Parameters
`subcmd` - the subcommand to initialize the flags to \
`valid_flags` - the list of flags

## Error
Throws a `CLIException` error if:
- the given subcommand is invalid
- a flag has an invalid prefix

## Notes
- Flags need to prefixed with `-` for single character flags such as `-h` and `--` with string flags such as `--help`
  
## Example
```
#include <iostream>
#include "cli.hpp"

void print(const std::unordered_set<std::string>& flags)
{
    for(const auto& i : flags) {
        std::cout << "\"" << i << "\" ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.addFlags({"-h", "--help"});
        cli.addFlags("add", {"--path"});
        cli.addFlags("status", {"-v", "--verbose"});
        cli.addFlags("remote add", {"--tags", "-h", "--help", "--verbose"});

        std::cout << "Empty Subcommand: ";
        print(cli.getValidFlags(""));

        std::cout << "add: ";
        print(cli.getValidFlags("add"));

        std::cout << "status: ";
        print(cli.getValidFlags("status"));

        std::cout << "remote add: ";
        print(cli.getValidFlags("remote add"));
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Output:
```
Empty Subcommand: "-h" "--help" 
add: "--path"
status: "-v" "--verbose"
remote add: "--tags" "-h" "--help" "--verbose"
```