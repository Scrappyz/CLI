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
        vector<string> values; // will hold the values of flags or the active subcommand
        if(subcmd.empty()) {
            // do stuff with empty subcommand
            if(cli.isFlagActive("-h") || cli.isFlagActive("--help")) {
                // do stuff with the "-h" and "--help" flag
            }   
        } else if(subcmd == "add") {
            // do stuff with "add" subcommand
            if(cli.isFlagActive("--path")) {
                // do stuff with the "--path" flag

                // get the value of the "--path" flag and limit it to 1
                values = cli.getValueOf("--path", 1); 

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

                // get the values of "-f" or "--fetch", whichever comes first
                values = cli.getValueOf({"-f", "--fetch"});

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

## Documentation
Additional documentation is available here:
- [CLI Library](docs/CLI/)
- [Tutorials](docs/Tutorials/)