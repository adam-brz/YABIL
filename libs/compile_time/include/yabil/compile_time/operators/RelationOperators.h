#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/compile_time/BigIntData.h>

#include <cstddef>

namespace yabil::compile_time
{

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool abs_lower(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool abs_greater(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                           const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator==(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator!=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator<(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator<=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator>(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator>=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

}  // namespace yabil::compile_time