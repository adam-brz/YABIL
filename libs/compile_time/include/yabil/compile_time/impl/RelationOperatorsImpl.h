#pragma once

#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/operators/RelationOperators.h>

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
    return (self.is_zero() && other.is_zero()) ||
           (std::ranges::equal(impl::normalize(SelfData), impl::normalize(OtherData)) && SelfSign == OtherSign);
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
        return true;
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
        return !self.is_zero() || !other.is_zero();
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