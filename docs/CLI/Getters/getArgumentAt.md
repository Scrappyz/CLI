## CLI::getArgumentAt

| |
| --- |
| const std::string& getArgumentAt(int index) |
| std::string getArgumentAt(int start, int end) |

Returns the argument at the given index or a concatenation of the arguments at the given range indexes.

## Parameters
`index` - the index of the argument
`start` - the start of the range \
`end` - end of the range

## Return Value
Returns the argument at the given index. Returns the concatenation of the arguments at the given range.

## Error
Throws a `CLIException` error if `start` is out of range.

## Example
```
#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    try {
        cli.setValidSubcommands({"add", "status", "remote add"});
        cli.setValidFlags("remote add", {"--tags"});

        std::cout << cli.getArgumentAt(1) << std::endl;
        std::cout << cli.getArgumentAt(1, 4) << std::endl;
    } catch(const CLIException& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

Input:
```
CLI.exe remote add --tags mytag
```

Output:
```
remote
remote add --tags mytag
```