#include <yabil/bigint/BigInt.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/crypto/Random.h>
#include <yabil/math/Math.h>

#include <iostream>

using namespace yabil;
using namespace yabil::bigint;
using namespace yabil::compile_time::literals;

int main()
{
    const BigInt a{"129012891241242430011215125293"};
    const BigInt b = 128391203124124124823_bi;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    std::cout << "a*b = " << a * b << std::endl;
    std::cout << "a/b = " << a / b << std::endl;
    std::cout << "pow(a,log2(b)) = " << math::pow(a, BigInt(math::log2_int(b))) << std::endl;
    std::cout << "random_prime(32-bits) = " << crypto::random::random_prime(32) << std::endl;
    return 0;
}
