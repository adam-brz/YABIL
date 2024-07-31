#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator+(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize, SelfData> &self);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator*(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator/(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator%(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

}  // namespace yabil::compile_time
