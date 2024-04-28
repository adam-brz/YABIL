#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/Parallel.h>

#include "Arithmetic.h"
#include "ParallelImpl.h"

namespace yabil::bigint::parallel
{

BigInt add(const BigInt &a, const BigInt &b)
{
    if (a.get_sign() == b.get_sign())
    {
        return BigInt(parallel_add_unsigned(a.raw_data(), b.raw_data()), a.get_sign());
    }

    const auto [greater, lower] = get_greater_lower(a, b);
    const Sign new_sign = ((greater == &a) == (a.get_sign() == Sign::Plus)) ? Sign::Plus : Sign::Minus;
    return BigInt(plain_sub(greater->raw_data(), lower->raw_data()), new_sign);
}

BigInt multiply(const BigInt &a, const BigInt &b)
{
    return BigInt(parallel_karatsuba(a.raw_data(), b.raw_data()),
                  (a.get_sign() == b.get_sign()) ? Sign::Plus : Sign::Minus);
}

}  // namespace yabil::bigint::parallel
