#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>
#include <yabil/compile_time/detail/operators/RelationOperators.h>
#include <yabil/compile_time/impl/Utils.h>

#include <algorithm>
#include <cstddef>

namespace yabil::compile_time
{

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool abs_lower()
{
    return SelfSize < OtherSize ||
           (SelfSize == OtherSize &&
            std::lexicographical_compare(SelfData.crbegin(), SelfData.crend(), OtherData.crbegin(), OtherData.crend()));
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool abs_greater()
{
    return abs_lower<OtherSign, OtherSize, OtherData, SelfSign, SelfSize, SelfData>();
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool abs_lower(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &)
{
    return abs_lower<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>();
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool abs_greater(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                           const ConstBigInt<OtherSign, OtherSize, OtherData> &)
{
    return abs_lower<OtherSign, OtherSize, OtherData, SelfSign, SelfSize, SelfData>();
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator==(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
{
    constexpr auto normalized_self = impl::normalize<SelfSize, SelfData>();
    constexpr auto normalized_other = impl::normalize<OtherSize, OtherData>();
    if constexpr (normalized_self.size() != normalized_other.size())
    {
        return false;
    }
    else
    {
        return (self.is_zero() && other.is_zero()) || (normalized_self == normalized_other && SelfSign == OtherSign);
    }
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator!=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
{
    return !(self == other);
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator<(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
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
        return !(self.is_zero() && other.is_zero());
    }
    else
    {
        return false;
    }
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator<=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
{
    return (self < other) || (self == other);
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator>(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
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
        return !(self.is_zero() && other.is_zero());
    }
    else
    {
        return false;
    }
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator>=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
{
    return (self > other) || (self == other);
}

}  // namespace yabil::compile_time
