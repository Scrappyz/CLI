## CLI::getArguments

| |
| --- |
| const std::vector\<std::string>& getArguments() |
| std::vector\<std::string> getArguments(int start) |
| std::vector\<std::string> getArguments(int start, int end) |

Returns all arguments or a range of arguments.

## Parameters
`start` - the starting index
`end` - the end index

## Return Value
Returns all arguments if no range is provided. Returns all arguments starting from `start` up to the end of the argument list if no `end` index is provided. Returns all arguments up to the `start` to `end` if both index are provided.

## Error
Throws a `CLIException` if either index is out of range.

## Example
```
#include <iostream>
#include "cli.hpp"

void printArgs(const std::vector<std::string>& args)
{
    for(int i = 0; i < args.size(); i++) {
        std::cout << args[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags({"-h", "--help"});
        cli.setValidFlags("add", {"--path"});
        cli.setValidFlags("status", {"-v", "--verbose"});
        cli.setValidFlags("remote add", {"--tags", "-h", "--help", "--verbose"});

        printArgs(cli.getArguments());
        printArgs(cli.getArguments(2));
        printArgs(cli.getArguments(1, 3));
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe remote add --tags mytags
```

Output:
```
D:\Documents\Codes\VS Code\C++\CLI\bin\Debug\CLI.exe remote add --tags mytags 
add --tags mytags
remote add --tags
```