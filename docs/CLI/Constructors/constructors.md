## CLI::CLI
Defined in header `cli.hpp`

| |
| --- | 
| CLI() | 
| CLI(int argc, char** argv) |
| CLI(const std::vector\<std::string>& args) |

Creates the `CLI` object.

## Example
```
#include <iostream>
#include "cli.hpp"

void printArgs(const std::vector<std::string>& args)
{
    for(int i = 0; i < args.size(); i++) {
        std::cout << args[i] << " ";
    }
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    printArgs(cli.getArguments());
    
    return 0;
}
```

Input:
```
CLI.exe hello world 
```

Output:
```
CLI.exe hello world 
```