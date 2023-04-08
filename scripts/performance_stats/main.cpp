#include <gmp.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/math/Math.h>

#include <fstream>
#include <iostream>

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
    yabil::math::factorial(n);
}

int main()
{
    std::ofstream gmp_out("gmp_factorial.csv");
    std::ofstream yabil_out("yabil_factorial.csv");

    profile_function(gmp_factorial, gmp_out, 0, 1000, 200);
    profile_function(yabil_factorial, yabil_out, 0, 1000, 200);

    return 0;
}
