#include <yabil/bigint/BigInt.h>

#include <iostream>

using namespace yabil::bigint;

int main()
{
    const BigInt a{"129012891241242430011215125293"};
    const BigInt b{"128391203124124124823"};
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "a*b = " << a * b << std::endl;
    std::cout << "a/b = " << a / b << std::endl;
    return 0;
}
