#include <gmp.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/crypto/Utils.h>

#include "profile_utils.h"

void gmp_factorial(int n)
{
    mpz_t number;
    mpz_init(number);
    mpz_fac_ui(number, n);
    mpz_clear(number);
}

void yabil_factorial(int n)
{
    yabil::crypto::factorial(static_cast<int>(n));
}

int main()
{
    profile_function(gmp_factorial, std::cout, 0, 200, 4);
    return 0;
}
