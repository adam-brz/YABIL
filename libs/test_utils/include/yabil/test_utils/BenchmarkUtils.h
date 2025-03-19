#pragma once

#include <concepts>
#include <cstddef>
#include <limits>
#include <random>
#include <string>
#include <vector>

namespace yabil::test_utils
{

std::string random_str_digits(std::size_t number_of_digits, int base);

template <std::unsigned_integral NumberType>
std::vector<NumberType> random_digits(std::size_t number_of_64_bit_digits)
{
    constexpr int seed = 44;
    static std::mt19937 gen(seed);
    static std::uniform_int_distribution<uint64_t> dist(1, std::numeric_limits<NumberType>::max());

    std::vector<NumberType> generated;

    const auto number_of_digits_to_generate = number_of_64_bit_digits * 64 / (sizeof(NumberType) * 8);
    generated.reserve(number_of_digits_to_generate);

    for (std::size_t i = 0; i < number_of_digits_to_generate; i++)
    {
        generated.push_back(dist(gen));
    }

    generated.back() |= static_cast<NumberType>(1) << (sizeof(NumberType) * 8 - 1);
    return generated;
}

}  // namespace yabil::test_utils
