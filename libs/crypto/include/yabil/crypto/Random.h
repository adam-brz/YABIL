#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstdint>

/// @brief Pseudo-random big integer generation functionalities
namespace yabil::crypto::random
{

/// @brief Generate large pseudo-random integer.
/// @param number_of_bits Number of bits to generate
/// @param top_two Force most-significant bit to value of 1
/// @param bottom_odd Force least-significant bit to value of 1
/// @return Pseudo-random \p BigInt
yabil::bigint::BigInt random_bigint(uint64_t number_of_bits = 64, bool top_two = false, bool bottom_odd = false);

/// @brief Generate large pseudo-random integer.
/// @param min Minimum value to generate
/// @param max Maximum value to generate
/// @return Pseudo-random \p BigInt
yabil::bigint::BigInt random_bigint(const yabil::bigint::BigInt &min, const yabil::bigint::BigInt &max);

/// @brief Generate large random prime number.
/// @param number_of_bits Number of bits for prime number
/// @return Pseudo-random prime \p BigInt
yabil::bigint::BigInt random_prime(uint64_t number_of_bits = 1024);

}  // namespace yabil::crypto::random
