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
    yabil::crypto::utils::factorial(n);
}

int main()
{
    profile_function(gmp_factorial, std::cout, 0, 200, 4);
    profile_function(yabil_factorial, std::cout, 0, 200, 4);
    return 0;
}
