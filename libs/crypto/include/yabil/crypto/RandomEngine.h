#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/crypto/crypto_export.h>

#include <cstdint>
#include <memory>
#include <random>

#include "yabil/bigint/BigIntBase.h"

namespace yabil::crypto::random
{

/// @brief Class providing functionalities for generation of random large big integer numbers.
/// @headerfile RandomEngine.h <yabil/crypto/RandomEngine.h>
class RandomEngine
{
private:
    class Impl;
    std::unique_ptr<Impl> impl;

public:
    /// @brief Create random engine and seed it with specified value.
    /// @param seed Seed for random engine. By default will use @p std::random_device
    RandomEngine(const std::size_t seed = std::random_device{}());

    ~RandomEngine();

    /// @brief Generate large pseudo-random integer.
    /// @param number_of_bits Number of bits to generate
    /// @param top_two Force most-significant bit to value of 1
    /// @param bottom_odd Force least-significant bit to value of 1
    /// @return Pseudo-random \p BigInt
    YABIL_CRYPTO_EXPORT yabil::bigint::BigInt random_bigint(uint64_t number_of_bits = 64, bool top_two = false,
                                                            bool bottom_odd = false);

    /// @brief Generate large pseudo-random integer.
    /// @param min Minimum value to generate
    /// @param max Maximum value to generate
    /// @return Pseudo-random \p BigInt
    YABIL_CRYPTO_EXPORT yabil::bigint::BigInt random_bigint(const yabil::bigint::BigInt &min,
                                                            const yabil::bigint::BigInt &max);

    /// @brief Generate large random prime number.
    /// @param number_of_bits Number of bits for prime number
    /// @return Pseudo-random prime \p BigInt
    YABIL_CRYPTO_EXPORT yabil::bigint::BigInt random_prime(uint64_t number_of_bits = 1024);

    /// @brief Generate single random digit using specified distribution.
    /// @param distribution Standard random distribution to use.
    YABIL_CRYPTO_EXPORT bigint::bigint_base_t random_digit(
        std::uniform_int_distribution<yabil::bigint::bigint_base_t> distribution =
            std::uniform_int_distribution<yabil::bigint::bigint_base_t>{});
};

}  // namespace yabil::crypto::random
