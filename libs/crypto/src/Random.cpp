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
            const auto mod = prime_candidate % yabil::bigint::BigInt(primes()[i]);
            if (mod.is_zero() ||
                (number_of_bits <= 31 && (yabil::bigint::BigInt(primes()[i] * primes()[i]) > prime_candidate)))
            {
                good_candidate = false;
                break;
            }
        }

        if (good_candidate)
        {
            return prime_candidate;
        }
    }
}

bool miller_rabin_test(const yabil::bigint::BigInt &prime_candidate)
{
    // const yabil::bigint::BigInt first_even_component = prime_candidate - yabil::bigint::BigInt(1);
    // yabil::bigint::BigInt even_component = first_even_component;

    // uint64_t max_divisions_by_two = 0;
    // while (even_component.is_even())
    // {
    //     even_component >>= 1;
    //     max_divisions_by_two += 1;
    // }

    // const auto is_trial_composite = [&](const yabil::bigint::BigInt &round_tester)
    // {
    //     if (yabil::math::pow(round_tester, even_component, prime_candidate) == yabil::bigint::BigInt(1))
    //     {
    //         return false;
    //     }
    //     for (uint64_t i = 0; i < max_divisions_by_two; ++i)
    //     {
    //         if (yabil::math::pow(round_tester, (yabil::bigint::BigInt(1) << i) * even_component, prime_candidate) ==
    //             first_even_component)
    //         {
    //             return false;
    //         }
    //     }
    //     return true;
    // };

    //
    // for (int i = 0; i < number_of_rabin_trials; ++i)
    // {
    //     const yabil::bigint::BigInt round_tester = random_bigint(yabil::bigint::BigInt(2), prime_candidate);
    //     if (is_trial_composite(round_tester))
    //     {
    //         return false;
    //     }
    // }

    const yabil::bigint::BigInt first_even_component = prime_candidate - yabil::bigint::BigInt(1);
    yabil::bigint::BigInt x;

    // 1. Calculate largest integer 'a' such that 2^a divides prime_candidate-1
    int a = 1;
    while (!first_even_component.get_bit(a))
    {
        a += 1;
    }

    // 2. m = (w-1) / 2^a
    const yabil::bigint::BigInt m = first_even_component >> a;
    constexpr int number_of_rabin_trials = 64;

    for (int i = 0; i < number_of_rabin_trials; ++i)
    {
        /* (Step 4.1) obtain a Random string of bits b where 1 < b < w-1 */
        const yabil::bigint::BigInt round_tester =
            random_bigint(yabil::bigint::BigInt(2), prime_candidate - yabil::bigint::BigInt(2));

        /* (Step 4.5) z = b^m mod w */
        yabil::bigint::BigInt z = yabil::math::pow(round_tester, m, prime_candidate);

        /* (Step 4.6) if (z = 1 or z = w-1) */
        if (z == yabil::bigint::BigInt(1) || z == first_even_component) break;

        bool z_is_prime_minus_one = false;
        for (int j = 1; j < a; ++j)
        {
            /* (Step 4.7.1 - 4.7.2) x = z. z = x^2 mod w */
            x = z;
            z = yabil::math::pow(x, bigint::BigInt(2), prime_candidate);
            if (z == first_even_component)
            {
                z_is_prime_minus_one = true;
                break;
            }
            if (z == bigint::BigInt(1)) return false;
        }

        if (!z_is_prime_minus_one)
        {
            x = z;
            z = yabil::math::pow(x, bigint::BigInt(2), prime_candidate);
            if (z == bigint::BigInt(1)) return false;
            x = z;
        }
    }

    return true;
}

}  // namespace

yabil::bigint::BigInt random_bigint(uint64_t number_of_bits, bool top_two, bool bottom_odd)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(0.5);
    std::vector<yabil::bigint::bigint_base_t> raw_bigint_data;

    constexpr size_t chunk_size_bits = sizeof(yabil::bigint::bigint_base_t) * 8;
    const size_t bigint_chunks_count = number_of_bits / chunk_size_bits;
    raw_bigint_data.reserve(bigint_chunks_count + 1);

    for (size_t i = 0; i < bigint_chunks_count; i++)
    {
        yabil::bigint::bigint_base_t chunk = 0;
        for (size_t j = 0; j < chunk_size_bits; ++j)
        {
            chunk |= static_cast<yabil::bigint::bigint_base_t>(dist(gen)) << j;
        }
        raw_bigint_data.push_back(chunk);
    }

    yabil::bigint::bigint_base_t last_chunk = 0;
    for (size_t i = 0; i < number_of_bits % chunk_size_bits; i++)
    {
        last_chunk |= static_cast<yabil::bigint::bigint_base_t>(dist(gen)) << i;
    }

    if (last_chunk)
    {
        raw_bigint_data.push_back(last_chunk);
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
    const uint64_t max_bits = max.byte_size() * 8;
    return min + (random_bigint(max_bits) % (max - min + yabil::bigint::BigInt(1)));
}

yabil::bigint::BigInt random_prime(uint64_t number_of_bits)
{
    if (number_of_bits < 2)
    {
        throw std::invalid_argument("There is no prime of 2 bits size");
    }

    while (true)
    {
        yabil::bigint::BigInt prime_candidate = probable_prime(number_of_bits);
        if (miller_rabin_test(prime_candidate))
        {
            return prime_candidate;
        }
    }
}

}  // namespace yabil::crypto::random
