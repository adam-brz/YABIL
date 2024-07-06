#pragma once

#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/operators/RelationOperators.h>

#include <algorithm>
#include <cstddef>

namespace yabil::compile_time
{

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval bool abs_lower(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    return SelfSize < OtherSize ||
           (SelfSize == OtherSize && std::lexicographical_compare(self.data.crbegin(), self.data.crend(),
                                                                  other.data.crbegin(), other.data.crend()));
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval bool abs_greater(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    return abs_lower(other, self);
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval bool operator==(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    return (self.is_zero() && other.is_zero()) ||
           (std::ranges::equal(detail::normalize(self.data), detail::normalize(other.data)) && SelfSign == OtherSign);
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval bool operator!=(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    return !(self == other);
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval bool operator<(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    if constexpr (SelfSign == Sign::Minus && OtherSign == Sign::Minus)
    {
        return abs_greater(self, other);
    }
    else if constexpr (SelfSign == Sign::Plus && OtherSign == Sign::Plus)
    {
        return abs_lower(self, other);
    }
    else if constexpr (SelfSign == Sign::Minus && OtherSign == Sign::Plus)
    {
        return true;
    }
    else
    {
        return false;
    }
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval bool operator<=(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    return (self < other) || (self == other);
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval bool operator>(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    if constexpr (SelfSign == Sign::Minus && OtherSign == Sign::Minus)
    {
        return abs_lower(self, other);
    }
    else if constexpr (SelfSign == Sign::Plus && OtherSign == Sign::Plus)
    {
        return abs_greater(self, other);
    }
    else if constexpr (SelfSign == Sign::Plus && OtherSign == Sign::Minus)
    {
        return !self.is_zero() || !other.is_zero();
    }
    else
    {
        return false;
    }
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval bool operator>=(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    return (self > other) || (self == other);
}

}  // namespace yabil::compile_time