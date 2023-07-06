## Classes
Defined in header `cli.hpp`
| | |
| --- | --- |
| CLI | class to use to parse command-line arguments |
| CLIException | custom exception for CLI class |

The `CLI` class stores the command-line arguments, the valid subcommands and their respective flags. 

## Constructors
| | |
| --- | --- |
| [(constructors)](Constructors/constructors.md) | constructs the `CLI` object |

## Getters
| | |
| --- | --- |
| [getProgramName]() | returns the program name |
| [getArguments]() | returns the argument list |
| [getActiveSubcommand]() | returns the current active subcommand |
| [getActiveSubcommandEndPosition]() | returns the position of the last word of the subcommand in the argument list |
| [getArgumentAt]() | returns the argument at a given position or range of positions |
| [getFlagPosition]() | returns the position of a valid flag in the argument list |
| [getValidSubcommands]() | returns the valid subcommands |
| [getActiveFlags]() | returns the flags that are present in the argument list |
| [getActiveFlagsAndPositions]() | returns the flags that are present and their positions in the argument list |
| [getActiveFlagIn]() | returns the first flag that is active in a given list of flags |
| [getAllActiveFlagsIn]() | returns all the active flags in a given list of flags |
| [getValueOf]() | returns a single value of a given flag |
| [getAllValuesOf]() | returns all the values of a given flag |
| [getActiveFlagsAndValues]() | returns all the active flags and their values |

## Setters
| | |
| --- | --- |
| [setArguments]() | initializes the argument list |
| [setValidSubcommands]() | initializes the valid subcommands |
| [setValidFlags]() | initializes the valid flags of a given valid subcommand |

## Lookup
| | |
| --- | --- |
| [isValidSubcommand]() | checks if a given string is a valid subcommand |
| [isActiveSubcommand]() | checks if a given string is the current active subcommand |
| [noActiveSubcommand]() | checks if no subcommand is active |
| [isFlagActive]() | checks if a given flag or atleast one flag in a given list of flags is present in the argument list |
| [isValidFlag]() | checks if a given flag is a valid flag of a given subcommand |

## Modifiers
| | |
| --- | --- |
| [clear]() | clears all data in a `CLI` object |