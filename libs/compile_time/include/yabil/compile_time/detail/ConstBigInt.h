#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/compile_time/detail/BigIntData.h>

namespace yabil::compile_time
{

using Sign = bigint::Sign;

static inline constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;

/// @brief Big integer class for arbitrary size unsigned integer numbers.
/// @details All computations can be performed in compile time.
/// @headerfile ConstBigInt.h <yabil/compile_time/detail/ConstBigInt.h>
template <Sign NumberSign = Sign::Plus, std::size_t InternalSize = 1,
          BigIntData<InternalSize> InternalData = BigIntData<InternalSize>{}>
class ConstBigInt
{
public:
    static inline constexpr std::size_t size = InternalSize;
    static inline constexpr Sign sign = NumberSign;
    static inline constexpr BigIntData<InternalSize> data{InternalData};

public:
    static consteval std::size_t real_size();
    static consteval std::size_t byte_size();

    static consteval bool is_zero();
    static consteval bool is_negative();

    static bigint::BigInt to_bigint();
    operator bigint::BigInt() const;

    template <std::size_t n>
    static consteval bool get_bit();

    template <std::signed_integral OutType>
    static consteval OutType to();

    template <std::unsigned_integral OutType>
    static consteval OutType to();
};

}  // namespace yabil::compile_time
