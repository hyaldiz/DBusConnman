#include <iostream>
#include "ConnmanTechnology.hpp"

using namespace std;

int main()
{

    ConnmanTechnology tech(ConnmanTechnology::_technologyTypeBluetooth);

    tech.setPowered(false);

    while(1);

    return 0;
}
