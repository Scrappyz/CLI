## CLI::getAnyValue
Defined in header `cli.hpp`

| |
| --- |
| std::string getAnyValue(int occurance = 1, const std::unordered_set\<std::string>& excluded_flags = {}) |
| std::string getAnyValue(const std::unordered_set\<std::string>& excluded_flags, int occurance = 1) |

Returns any value in the argument list regardless of flag.

## Parameters
`excluded_flags` - flags to exclude
`occurance` - which value to get

## Return Value
Returns any value in the argument list.

## Example
```
