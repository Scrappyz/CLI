#include <iostream>
#include <algorithm>
#include <vector>

void print(const std::vector<std::string>& v)
{
    for(int i = 0; i < v.size(); i++) {
        std::cout << v[i] << std::endl;
    }
}

int main()
{
    std::vector<std::string> v = {"getProgramName", "getArguments", "getActiveSubcommand", "getActiveSubcommandEndPosition",
    "getArgumentAt", "getFlagPosition", "getValidSubcommands", "getValidFlags", "getActiveFlags", "getActiveFlagsAndPositions", 
    "getActiveFlagIn", "getAllActiveFlagsIn", "getValueOf", "getAllValuesOf", "getActiveFlagsAndValues"};

    std::sort(v.begin(), v.end());
    print(v);
}