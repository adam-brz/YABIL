#include <yabil/test_utils/BenchmarkUtils.h>

#include <random>

namespace yabil::test_utils
{

std::string random_str_digits(std::size_t number_of_digits, int base)
{
    constexpr int seed = 44;
    static std::mt19937 gen(seed);
    static std::uniform_int_distribution<int> dist(0, base - 1);

    std::string generated;
    generated.reserve(number_of_digits);

    for (std::size_t i = 0; i < number_of_digits; i++)
    {
        const auto digit = dist(gen);
        if (digit < 10)
        {
            generated.push_back(static_cast<char>(digit + '0'));
        }
        else
        {
            generated.push_back(static_cast<char>(digit - 10 + 'A'));
        }
    }

    return generated;
}

}  // namespace yabil::test_utils
