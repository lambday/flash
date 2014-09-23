#include <test.h>
#include <iostream>

void Base::set(int val)
{
    std::cout << "Base::set(): " << val << std::endl;
}
