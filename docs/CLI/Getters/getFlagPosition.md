## CLI::getFlagPosition
Defined in header `cli.hpp`

| |
| --- |
| int getFlagPosition(const std::string& flag) |

Returns the index of a valid flag.

## Parameters
`flag` - the valid flag to return the position of

## Return Value
Returns the index of the given valid flag if it is present in the argument list, `-1` otherwise.

## Error
Throws a `CLIException` error if the given flag is not a valid flag.

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

        std::cout << cli.getFlagPosition("--tags") << std::endl;
        std::cout << cli.getFlagPosition("--invalid-flag") << std::endl;
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
3
[Error][getFlagPosition] "--invalid-flag" is not a valid flag
```