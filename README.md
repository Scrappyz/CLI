## CLI
The CLI is a lightweight command-line parsing library in C++ that is designed to simplify the development of console applications. It is implemented as a header-only library, which further enhances its convenience and ease of use.

<!-- omit in toc -->
## Table Of Contents
- [CLI](#cli)
- [Features](#features)
- [Dependencies](#dependencies)
  - [Minimum C++ Version](#minimum-c-version)
  - [Includes](#includes)
- [Installation](#installation)
- [Usage](#usage)
- [Documentation](#documentation)

## Features
- **Customizable Subcommands:** Declare and customize the subcommands available in your application.
- **Customizable Flags:** Define and customize the flags associated with each subcommand.
- **Extractable Values:** Conveniently extract and access the values of command-line arguments.
- **User-Friendly:** Intuitive and easy-to-understand syntax for streamlined usage.

## Dependencies
### Minimum C++ Version
- [C++11](https://en.cppreference.com/w/cpp/11)

### Includes
- [\<string>](https://en.cppreference.com/w/cpp/header/string)
- [\<vector>](https://en.cppreference.com/w/cpp/container/vector)
- [\<unordered_set>](https://en.cppreference.com/w/cpp/header/unordered_set)
- [\<unordered_map>](https://en.cppreference.com/w/cpp/header/unordered_map)

## Installation
CLI is a header-only library so all you need to do is:
1. Download `cli.hpp` in [releases](https://github.com/Scrappyz/CLI/releases)
2. Place `cli.hpp` in the include folder of your project
3. Include the `cli.hpp` header in your code

## Usage
1. **Pass the command-line arguments:** We first pass the command-line arguments into the `CLI` object to initialize its argument list.
```
#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    // more code
```

2. **Catch Exceptions:** Create a try-catch block that catches a `CLIException` object. (This step is optional)
```
#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        // more code
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

3. **Initialize the available subcommands and flags:** Each subcommand will have its own set of flags. Use the [addSubcommands()](docs/CLI/Modifiers/addSubcommands.md) method to declare the available subcommands and use [addFlags()](docs/CLI/Modifiers/addFlags.md) to declare their flags. All flags need to be prefixed with a `-` for single character flags and `--` for string flags. Once the subcommands and flags have been declared, initialize them with the [init()](docs/CLI/Modifiers/init.md) method.
```
#include <iostream>
#include "cli.hpp"

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

        // initialize the subcommands and flags
        cli.init();

        // more code
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

4. **Use the methods of the `CLI` object to run your program accordingly:** We first store the active subcommand in `subcmd` with [getActiveSubcommand()](docs/CLI/Getters/getActiveSubcommand.md) so we can use it to check which subcommand is active. The active subcommand is the subcommand that is currently in the command-line arguments. The variable `value` and `all_values` will be used to store the values of the command-line arguments. Once the subcommand in the command-line arguments has been determined, we can check the flags that are present in the argument list with [isFlagActive()](docs/CLI/Lookup/isFlagActive.md) and extract its values with either [getValueOf()](docs/CLI/Getters/getValueOf.md) or [getAllValuesOf()](docs/CLI/Getters/getAllValuesOf.md).
```
#include <iostream>
#include "cli.hpp"

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

        // initialize the subcommands and flags
        cli.init();

        std::string subcmd = cli.getActiveSubcommand(); // the current subcommand
        std::string value; // will hold a single value of a flag
        std::vector<std::string> all_values; // will hold all the values of a flag
        if(subcmd.empty()) {
            // do stuff with empty subcommand

            if(cli.isFlagActive("-h") || cli.isFlagActive("--help")) {
                // do stuff with the "-h" and "--help" flag
            }   
        } else if(subcmd == "add") {
            // do stuff with "add" subcommand
            if(cli.isFlagActive("--path")) {
                // do stuff with the "--path" flag

                // get the first value of "--path"
                value = cli.getValueOf("--path", 1);

                // do stuff with the value
            }
        } else if(subcmd == "status") {
            // do stuff with "status" subcommand

            if(cli.isFlagActive("-v") || cli.isFlagActive("--verbose")) {
                // do stuff with the "verbose" flag
            }
        } else if(subcmd == "remote add") {
            // do stuff with "remote add" subcommand

            // same as `cli.isFlagActive("-f") || cli.isFlagActive("--fetch")`
            if(cli.isFlagActive({"-f", "--fetch"})) {
                // do stuff with "fetch" flag

                // get all the values of "-f" or "--fetch", whichever comes first
                all_values = cli.getAllValuesOf({"-f", "--fetch"});

                // do stuff with values
            }
        }
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Documentation
Additional documentation for each methods are available here:
- [CLI](docs/CLI/cli.md)