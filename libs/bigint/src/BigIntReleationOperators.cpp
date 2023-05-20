#include <yabil/bigint/BigInt.h>

namespace yabil::bigint
{

bool BigInt::operator==(const BigInt &other) const
{
    return (is_zero() && other.is_zero()) || (sign == other.sign && data == other.data);
}

bool BigInt::operator!=(const BigInt &other) const
{
    return !(*this == other);
}

bool BigInt::operator<(const BigInt &other) const
{
    if (sign == Sign::Minus && other.sign == Sign::Minus)
    {
        return abs_greater(other);
    }
    if (sign == Sign::Plus && other.sign == Sign::Plus)
    {
        return abs_lower(other);
    }
    if (sign == Sign::Minus && other.sign == Sign::Plus)
    {
        return true;
    }
    return false;
}

bool BigInt::operator<=(const BigInt &other) const
{
    return (*this < other) || (*this == other);
}

bool BigInt::operator>(const BigInt &other) const
{
    if (sign == Sign::Minus && other.sign == Sign::Minus)
    {
        return abs_lower(other);
    }
    if (sign == Sign::Plus && other.sign == Sign::Plus)
    {
        return abs_greater(other);
    }
    if (sign == Sign::Plus && other.sign == Sign::Minus)
    {
        return !is_zero() || !other.is_zero();
    }
    return false;
}

bool BigInt::operator>=(const BigInt &other) const
{
    return (*this > other) || (*this == other);
}

}  // namespace yabil::bigint
