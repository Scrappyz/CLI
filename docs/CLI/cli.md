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
| [getFlagPosition](Getters/getFlagPosition.md) | returns the position of a flag in the argument list |
| [getSubcommands](Getters/getSubcommands.md) | returns the list of subcommands |
| [getFlags](Getters/getFlags.md) | returns the list of flags in a given subcommand
| [getActiveFlags](Getters/getActiveFlags.md) | returns the flags that are present in the argument list |
| [getActiveFlagAmong](Getters/getActiveFlagAmong.md) | returns the first flag that is active in a given list of flags |
| [getAllActiveFlagsAmong](Getters/getAllActiveFlagsAmong.md) | returns all the active flags in a given list of flags |
| [getValueOf](Getters/getValueOf.md) | returns a single value of a given flag |
| [getAllValuesOf](Getters/getAllValuesOf.md) | returns all the values of a given flag |

## Setters
| | |
| --- | --- |
| [setArguments](Setters/setArguments.md) | initializes the argument list |

## Lookup
| | |
| --- | --- |
| [isSubcommandValid](Lookup/isSubcommandValid.md) | checks if a given string is a valid subcommand |
| [isActiveSubcommand](Lookup/isActiveSubcommand.md) | checks if a given string is the current active subcommand |
| [noActiveSubcommand](Lookup/noActiveSubcommand.md) | checks if no subcommand is active |
| [isFlagActive](Lookup/isFlagActive.md) | checks if a given flag or atleast one flag in a given list of flags is present in the argument list |
| [areFlagsActive](Lookup/areFlagsActive.md) | checks if all the given flags are active |
| [isFlagValid](Lookup/isFlagValid.md) | checks if a given flag is a valid flag of a given subcommand |

## Modifiers
| | |
| --- | --- |
| [addSubcommands](Modifiers/addSubcommands.md) | adds the available subcommands |
| [addGlobalFlags](Modifiers/addGlobalFlags.md) | adds the flags to all subcommands |
| [addFlags](Modifiers/addFlags.md) | adds the flags to a given subcommand |
| [clear](Modifiers/clear.md) | clears all data in a `CLI` object |