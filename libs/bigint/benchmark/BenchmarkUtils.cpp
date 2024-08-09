#include "BenchmarkUtils.h"

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

#include <limits>
#include <random>

namespace yabil::bigint::benchmark_utils
{

std::vector<bigint_base_t> random_digits(std::size_t number_of_digits)
{
    constexpr int seed = 44;
    static std::mt19937 gen(seed);
    static std::uniform_int_distribution<bigint_base_t> dist(1, std::numeric_limits<bigint_base_t>::max());

    std::vector<bigint_base_t> generated;
    generated.reserve(number_of_digits);

    for (std::size_t i = 0; i < number_of_digits; i++)
    {
        generated.push_back(dist(gen));
    }

    generated.back() |= static_cast<bigint_base_t>(1) << (bigint_base_t_size_bits - 1);
    return generated;
}

}  // namespace yabil::bigint::benchmark_utils