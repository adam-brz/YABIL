#include <yabil/bigint/Parallel.h>
#include <yabil/crypto/Random.h>
#include <yabil/math/Math.h>

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <random>
#include <stdexcept>
#include <vector>

#include "Primes.h"

namespace yabil::crypto::random
{

namespace
{

int trial_divisions(uint64_t bits)
{
    if (bits <= 512)
        return 64;
    else if (bits <= 1024)
        return 128;
    else if (bits <= 2048)
        return 384;
    else if (bits <= 4096)
        return 1024;
    return 2048;
}

yabil::bigint::BigInt probable_prime(uint64_t number_of_bits)
{
    int trial_division_count = trial_divisions(number_of_bits);

    while (true)
    {
        yabil::bigint::BigInt prime_candidate = random_bigint(number_of_bits, true, true);
        bool good_candidate = true;

        for (int i = 1; i < trial_division_count; ++i)
        {
            const auto mod = prime_candidate % primes()[i];
            if (mod == 0)
            {
                good_candidate = prime_candidate.is_uint64()
                                     ? (prime_candidate.to_uint() == static_cast<uint64_t>(primes()[i]))
                                     : false;
                break;
            }
        }

        if (good_candidate)
        {
            return prime_candidate;
        }
    }
}

bool miller_rabin_round(const yabil::bigint::BigInt &prime_candidate,
                        const yabil::bigint::BigInt &prime_candidate_minus_one,
                        const yabil::bigint::BigInt &odd_component, int two_power_divisor)
{
    const yabil::bigint::BigInt round_tester =
        random_bigint(yabil::bigint::BigInt(2), prime_candidate - yabil::bigint::BigInt(2));

    yabil::bigint::BigInt z = yabil::math::pow(round_tester, odd_component, prime_candidate);
    if (z == yabil::bigint::BigInt(1) || z == prime_candidate_minus_one)
    {
        return true;
    }

    for (int j = 1; j < two_power_divisor; ++j)
    {
        z = yabil::math::pow(z, bigint::BigInt(2), prime_candidate);
        if (z == bigint::BigInt(1)) return false;
        if (z == prime_candidate_minus_one) return true;
    }

    return false;
}

bool miller_rabin_test(const yabil::bigint::BigInt &prime_candidate)
{
    const yabil::bigint::BigInt prime_candidate_minus_one = prime_candidate - yabil::bigint::BigInt(1);
    int two_power_divisor = 1;
    while (!prime_candidate_minus_one.get_bit(two_power_divisor))
    {
        ++two_power_divisor;
    }

    const yabil::bigint::BigInt odd_component = prime_candidate_minus_one >> two_power_divisor;
    constexpr int number_of_rabin_trials = 64;

    for (int i = 0; i < number_of_rabin_trials; ++i)
    {
        if (!miller_rabin_round(prime_candidate, prime_candidate_minus_one, odd_component, two_power_divisor))
            return false;
    }

    return true;
}

}  // namespace

yabil::bigint::BigInt random_bigint(uint64_t number_of_bits, bool top_two, bool bottom_odd)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<yabil::bigint::bigint_base_t> dist;

    constexpr std::size_t chunk_size_bits = sizeof(yabil::bigint::bigint_base_t) * 8;
    const std::size_t bigint_chunks_count = number_of_bits / chunk_size_bits;

    std::vector<yabil::bigint::bigint_base_t> raw_bigint_data;
    raw_bigint_data.reserve(bigint_chunks_count + 1);

    for (std::size_t i = 0; i < bigint_chunks_count; i++)
    {
        raw_bigint_data.push_back(dist(gen));
    }

    const std::size_t bigint_remaining_bits = number_of_bits % chunk_size_bits;
    if (bigint_remaining_bits)
    {
        raw_bigint_data.push_back(dist(gen) >> (chunk_size_bits - bigint_remaining_bits));
    }

    if (bottom_odd)
    {
        raw_bigint_data.front() |= 1;
    }

    if (top_two)
    {
        raw_bigint_data.back() |= static_cast<yabil::bigint::bigint_base_t>(1) << (chunk_size_bits - 1);
    }

    return yabil::bigint::BigInt(std::move(raw_bigint_data));
}

yabil::bigint::BigInt random_bigint(const yabil::bigint::BigInt &min, const yabil::bigint::BigInt &max)
{
    const uint64_t max_bits = max.byte_size() * 8 - std::countl_zero(max.raw_data().back());
    auto result = random_bigint(max_bits);

    if (result > max)
    {
        result -= max;
    }

    if (result < min)
    {
        result = min;
    }

    return result;
}

yabil::bigint::BigInt random_prime(uint64_t number_of_bits)
{
    constexpr unsigned max_iter = 128000;

    if (number_of_bits <= 2)
    {
        throw std::invalid_argument("There is no prime of 2 bits size");
    }

    for (unsigned i = 0; i < max_iter; ++i)
    {
        yabil::bigint::BigInt prime_candidate = probable_prime(number_of_bits);
        if (miller_rabin_test(prime_candidate))
        {
            return prime_candidate;
        }
    }

    throw std::runtime_error("Cannot generate prime in: " + std::to_string(max_iter) + " steps");
}

}  // namespace yabil::crypto::random
