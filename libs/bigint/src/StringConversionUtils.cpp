#include "StringConversionUtils.h"

#include <stdexcept>
#include <string>

namespace yabil::bigint
{

int get_digit_value(int digit)
{
    if (digit >= 'a' && digit <= 'f') return digit - 'a' + 10;
    return digit - '0';
}

char get_digit_char(int digit)
{
    if (digit >= 10 && digit <= 16) return static_cast<char>(digit + 'a' - 10);
    return static_cast<char>(digit + '0');
}

void check_conversion(char chr, unsigned converted, unsigned base)
{
    if (converted > base)
    {
        throw std::invalid_argument("Cannot convert character: " + std::string(1, chr) +
                                    " to number of base: " + std::to_string(base));
    }
}

}  // namespace yabil::bigint
