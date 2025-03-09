#include "BenchmarkUtils.h"

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

#include <limits>
#include <random>

namespace yabil::bigint::benchmark_utils
{

std::vector<bigint_base_t> random_digits(std::size_t number_of_64_bit_digits)
{
    constexpr int seed = 44;
    static std::mt19937 gen(seed);
    static std::uniform_int_distribution<bigint_base_t> dist(1, std::numeric_limits<bigint_base_t>::max());

    std::vector<bigint_base_t> generated;

    const auto number_of_digits_to_generate = number_of_64_bit_digits * 64 / BigInt::digit_size_bits;
    generated.reserve(number_of_digits_to_generate);

    for (std::size_t i = 0; i < number_of_digits_to_generate; i++)
    {
        generated.push_back(dist(gen));
    }

    generated.back() |= static_cast<bigint_base_t>(1) << (BigInt::digit_size_bits - 1);
    return generated;
}

std::string random_str_digits(std::size_t number_of_digits, int base)
{
    constexpr int seed = 44;
    static std::mt19937 gen(seed);
    static std::uniform_int_distribution<int> dist(0, base - 1);

    std::string generated;
    generated.reserve(number_of_digits);

    for (std::size_t i = 0; i < number_of_digits; i++)
    {
        generated.push_back(static_cast<char>(dist(gen) + '0'));
    }

    return generated;
}

}  // namespace yabil::bigint::benchmark_utils
