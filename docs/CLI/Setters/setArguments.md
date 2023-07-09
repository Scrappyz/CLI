## CLI::setArugments
Defined in header `cli.hpp`

| |
| --- |
| void setArguments(int argc, char** args) |
| void setArguments(const std::vector\<std::string>& args) |

Initializes the argument list in the `CLI` object.

## Parameters
`argc` - the argument count \
`args` - the argument list

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
    CLI cli;
    cli.setArguments(argc, argv);
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