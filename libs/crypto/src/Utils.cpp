#include <yabil/crypto/Utils.h>

namespace yabil::crypto::utils
{

yabil::bigint::BigInt factorial(unsigned n)
{
    yabil::bigint::BigInt result(1U);
    for (unsigned i = 1; i <= n; ++i)
    {
        result *= yabil::bigint::BigInt(i);
    }
    return result;
}

}  // namespace yabil::crypto::utils
