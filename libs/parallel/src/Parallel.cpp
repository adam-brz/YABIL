#include <yabil/bigint/BigInt.h>
#include <yabil/parallel/Parallel.h>

#include "ParallelImpl.h"

namespace yabil::parallel
{

bigint::BigInt add(const bigint::BigInt &a, const bigint::BigInt &b)
{
    if (a.get_sign() == b.get_sign())
    {
        return bigint::BigInt(parallel_add_unsigned(a.raw_data(), b.raw_data()), a.get_sign());
    }

    // TODO(.): Implement subtraction, add benchmarks
    return a + b;
}

bigint::BigInt mul(const bigint::BigInt &a, const bigint::BigInt &b)
{
    return bigint::BigInt(parallel_karatsuba(a.raw_data(), b.raw_data()),
                          (a.get_sign() == b.get_sign()) ? bigint::Sign::Plus : bigint::Sign::Minus);
}

}  // namespace yabil::parallel
