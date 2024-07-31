#include "SomeLib.h"
#include <iostream>

extern "C" void Hello()
{
    std::cout << "Hello" << std::endl;
}