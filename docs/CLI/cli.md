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
| [getProgramName](Getters/getProgramName.md) | returns the program name |
| [getArguments](Getters/getArguments.md) | returns the argument list |
| [getArgumentCount](Getters/getArgumentCount.md) | returns the size of the argument list |
| [getActiveSubcommand](Getters/getActiveSubcommand.md) | returns the current active subcommand |
| [getStartPosition](Getters/getStartPosition.md) | returns the index after the subcommand |
| [getArgumentAt](Getters/getArgumentAt.md) | returns the argument at a given position or range of positions |
| [getFlagPosition](Getters/getFlagPosition.md) | returns the position of a valid flag in the argument list |
| [getValidSubcommands](Getters/getValidSubcommands.md) | returns the valid subcommands |
| [getActiveFlags](Getters/getActiveFlags.md) | returns the flags that are present in the argument list |
| [getActiveFlagsAndPositions](Getters/getActiveFlagsAndPositions.md) | returns the flags that are present and their positions in the argument list |
| [getActiveFlagIn](Getters/getActiveFlagIn.md) | returns the first flag that is active in a given list of flags |
| [getAllActiveFlagsIn](Getters/getAllActiveFlagsIn.md) | returns all the active flags in a given list of flags |
| [getValueOf](Getters/getValueOf.md) | returns a single value of a given flag |
| [getAllValuesOf](Getters/getAllValuesOf.md) | returns all the values of a given flag |
| [getActiveFlagsAndValues](Getters/getActiveFlagsAndValues.md) | returns all the active flags and their values |

## Setters
| | |
| --- | --- |
| [setArguments](Setters/setArguments.md) | initializes the argument list |
| [setValidSubcommands](Setters/setValidSubcommands.md) | initializes the valid subcommands |
| [setValidFlags](Setters/setValidFlags.md) | initializes the valid flags of a given valid subcommand |

## Lookup
| | |
| --- | --- |
| [isValidSubcommand](Lookup/isValidSubcommand.md) | checks if a given string is a valid subcommand |
| [isActiveSubcommand](Lookup/isActiveSubcommand.md) | checks if a given string is the current active subcommand |
| [noActiveSubcommand](Lookup/noActiveSubcommand.md) | checks if no subcommand is active |
| [isFlagActive](Lookup/isFlagActive.md) | checks if a given flag or atleast one flag in a given list of flags is present in the argument list |
| [isValidFlag](Lookup/isValidFlag.md) | checks if a given flag is a valid flag of a given subcommand |

## Modifiers
| | |
| --- | --- |
| [clear](Modifiers/clear.md) | clears all data in a `CLI` object |