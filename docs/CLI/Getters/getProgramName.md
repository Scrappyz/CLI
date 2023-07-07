## CLI::getProgramName
Defined in header `cli.hpp`

| |
| --- |
| std::string getProgramName() | 

## Return Value
Returns the name of the current program.

## Notes
- Removes the file extension so `MyProgram.exe` will return as `MyProgram`.

## Example
```
#include <iostream>
#include "cli.hpp"

int main(int argc, char* argv[])
{
    CLI cli(argc, argv);
    std::cout << cli.getProgramName() << std::endl;

    return 0;
}
```

Input:
```
CLI.exe
```

Output:
```
CLI
```