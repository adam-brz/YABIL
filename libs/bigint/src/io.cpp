#include <yabil/bigint/io.h>

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
    in >> first;

    const Sign sign = (first == '-') ? Sign::Minus : Sign::Plus;
    const bool hasSign = (first == '-') || (first == '+');

    BigInt result;
    if (!hasSign)
    {
        int converted = get_digit_value(std::tolower(first));
        check_conversion(first, static_cast<unsigned>(converted), base);
        result = BigInt(converted);
    }

    for (auto it = std::istreambuf_iterator<char>(in); it != std::istreambuf_iterator<char>(); ++it)
    {
        const auto converted = get_digit_value(std::tolower(*it));
        check_conversion(*it, static_cast<unsigned>(converted), base);
        result *= BigInt(base);
        result += BigInt(converted);
    }

    result.sign = sign;
    bigint = std::move(result);

    return in;
}

}  // namespace yabil::bigint