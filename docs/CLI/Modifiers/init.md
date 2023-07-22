## CLI::init
Defined in header `cli.hpp`

| |
| --- |
| void init() |

Initializes the available subcommands and flags.

## Error
Throws a `CLIException` error if:
- an invalid flag is encountered in the argument list

## Example
```
#include <iostream>
#include "cli.hpp"

void print(const CLI& cli)
{
    std::cout << "Active Subcommand: \"" << cli.getActiveSubcommand() << "\"" << std::endl;
    std::unordered_set<std::string> active_flags = cli.getActiveFlags();

    if(active_flags.empty()) {
        std::cout << "[NO ACTIVE FLAGS]" << std::endl;
        return;
    }

    std::cout << "Active Flags: ";
    for(const auto& i : active_flags) {
        std::cout << "\"" + i << "\" "; 
    }
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        // declare the available subcommands in your program
        cli.addSubcommands({"add", "status", "remote add"});

        // declare the available flags for each subcommand
        cli.addGlobalFlags({"-h", "--help"}); // flags for all subcommands
        cli.addFlags("add", {"--path"}); // flags for "add" subcommand
        cli.addFlags("status", {"-v", "--verbose"}); // flags for "status" subcommand
        cli.addFlags("remote add", {"-f", "--fetch", "--tags"}); // flags for "remote add" subcommand

        std::cout << "========Before initialization========" << std::endl;
        print(cli);

        // initialize subcommands and flags
        cli.init();

        std::cout << "\n========After initialization========" << std::endl;
        print(cli);
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

Input:
```
CLI.exe status value1 -h -v=true --help
```

Output:
```
========Before initialization========
Active Subcommand: ""
[NO ACTIVE FLAGS]

========After initialization========
Active Subcommand: "status"
Active Flags: "-h" "--help" "-v"
```