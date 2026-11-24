#include <yabil/bigint/io.h>

#include <cctype>
#include <stdexcept>
#include <string>

#include "StringConversionUtils.h"

namespace yabil::bigint
{

std::ostream &operator<<(std::ostream &out, const BigInt &bigint)
{
    out << bigint.to_str();
    return out;
}

std::istream &operator>>(std::istream &in, BigInt &bigint)
{
    constexpr int base = 10;

    char first;
    if (!(in >> first))
    {
        return in;
    }

    const Sign sign = (first == '-') ? Sign::Minus : Sign::Plus;
    const bool hasSign = (first == '-') || (first == '+');

    BigInt result;
    if (!hasSign)
    {
        int converted = get_digit_value(std::tolower(static_cast<unsigned char>(first)));
        check_conversion(first, static_cast<unsigned>(converted), base);
        result = BigInt(converted);
    }

    const auto at_number_end = [&in]()
    {
        const auto next = in.peek();
        return next == std::char_traits<char>::eof() || std::isspace(next);
    };

    if (hasSign && at_number_end())
    {
        throw std::invalid_argument("Invalid number format");
    }

    while (!at_number_end())
    {
        const auto chr = static_cast<char>(in.get());
        const auto converted = get_digit_value(std::tolower(static_cast<unsigned char>(chr)));
        check_conversion(chr, static_cast<unsigned>(converted), base);
        result *= BigInt(base);
        result += BigInt(converted);
    }

    result.set_sign(sign);
    bigint = std::move(result);

    return in;
}

}  // namespace yabil::bigint
