## CLI
The CLI is a lightweight command-line parsing library in C++ that aims to simplify the development of console applications.

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
- **Customisable Subcommands:** You can declare the available subcommands of your application
- **Customisable Flags:** You can declare the available flags for each subcommand
- **Extractable Values:** Easily extract the values of your command-line arguments

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
This is a sample usage of the `CLI` library.
```
#include <iostream>
#include "cli.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        // declare the available subcommands in your program
        cli.setValidSubcommands({"add", "status", "remote add"});

        // declare the available flags for each subcommand
        cli.setValidFlags({"-h", "--help"}); // declare flag for empty subcommand
        cli.setValidFlags("add", {"-h", "--help", "--path"});
        cli.setValidFlags("status", {"-h", "--help", "-v", "--verbose"});
        cli.setValidFlags("remote add", {"-f", "--fetch", "--tags"});

        string subcmd = cli.getActiveSubcommand(); // the current subcommand
        string value; // will hold a single value of a flag
        vector<string> all_values; // will hold all the values of a flag
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
            if(cli.isFlagActive("-f") || cli.isFlagActive("--fetch")) {
                // do stuff with "fetch" flag

                // get all the values of "-f" or "--fetch", whichever comes first
                all_values = cli.getAllValuesOf({"-f", "--fetch"});

                // do stuff with values
            }
        }
    } catch(const CLIException& e) {
        cout << e.what() << endl;
        return 1;
    }
    
    return 0;
}
```
```
// declare the available subcommands in your program
cli.setValidSubcommands({"add", "status", "remote add"});

// declare the available flags for each subcommand
cli.setValidFlags({"-h", "--help"}); // declare flag for empty subcommand
cli.setValidFlags("add", {"-h", "--help", "--path"});
cli.setValidFlags("status", {"-h", "--help", "-v", "--verbose"});
cli.setValidFlags("remote add", {"-f", "--fetch", "--tags"});
```
First we declare the subcommands our program can take with `setValidSubcommands({"list", "of", "subcommands"})` then we declare the flags each subcommand can take with `setValidFlags("subcommand name", {"--list", "--of", "--flags"})`. 

```
string subcmd = cli.getActiveSubcommand(); // the current subcommand
string value; // will hold a single value of a flag
vector<string> all_values; // will hold all the values of a flag
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
    if(cli.isFlagActive("-f") || cli.isFlagActive("--fetch")) {
        // do stuff with "fetch" flag

        // get all the values of "-f" or "--fetch", whichever comes first
        all_values = cli.getAllValuesOf({"-f", "--fetch"});

        // do stuff with values
    }
}
```
We first store the active subcommand in `subcmd` so we can use it to check which subcommand is active. The variable `value` and `all_values` will be used to store the values of the command-line arguments. Once the active subcommand has been determined, we can check the flags that are present in the argument list with `isFlagActive("--flag")` and extract its values with either `getValueOf("--flag")` or `getAllValuesOf("--flag")`.

## Documentation
Additional documentation and tutorials on how to use are available here:
- [CLI Library](docs/CLI/)
- [Tutorials](docs/Tutorials/)