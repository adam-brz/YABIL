#include "utils.h"

#include <string>
#include <random>

std::string generate_random_number_string(int digits)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<unsigned> dist(1, 9);

    std::string number;
    number.reserve(digits);

    for(int i = 0; i < digits; ++i)
    {
        number.push_back('0' + dist(gen));
    }

    return number;
}