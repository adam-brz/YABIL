#include "utils.h"  // NOLINT

#include <random>
#include <string>

std::string generate_random_number_string(int64_t digits)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<unsigned> dist(1, 9);

    std::string number;
    number.reserve(digits);

    for (int64_t i = 0; i < digits; ++i)
    {
        number.push_back(static_cast<char>('0' + dist(gen)));
    }

    return number;
}
