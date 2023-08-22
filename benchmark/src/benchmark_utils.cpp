#include "benchmark_utils.h"  // NOLINT

#include <random>
#include <string>

std::string generate_random_number_string(int64_t digits)
{
    constexpr int seed = 17;
    static std::mt19937 gen(seed);
    static std::uniform_int_distribution<unsigned> dist(1, 9);

    std::string number;
    number.reserve(digits);

    for (int64_t i = 0; i < digits; ++i)
    {
        number.push_back(static_cast<char>('0' + dist(gen)));
    }

    return number;
}

std::pair<std::string, std::string> BaseBigIntBenchmark::generate_test_numbers(int size)
{
    static const std::string a = generate_random_number_string(number_max_size_digits);
    static const std::string b = generate_random_number_string(number_max_size_digits);
    return {a.substr(0, size), b.substr(0, size)};
}
