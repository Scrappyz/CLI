#include <iostream>
#include "cli.hpp"

using namespace std;

int main(int argc, char** argv)
{
    string str = "-h=";
    cout << str.find_first_of('=', 2) << endl;
    cout << str.substr(3, 6) << endl;
    return 0;
}