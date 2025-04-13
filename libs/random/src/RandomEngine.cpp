#include <yabil/math/Math.h>
#include <yabil/random/RandomEngine.h>

#include <bit>
#include <cassert>
#include <cmath>
#include <cstring>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "Primes.h"

namespace yabil::random
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

}  // namespace

class RandomEngine::Impl
{
private:
    std::mt19937 generator;
    std::uniform_int_distribution<yabil::bigint::bigint_base_t> uniform_dist;

public:
    explicit Impl(unsigned seed) : generator(seed) {}
    yabil::bigint::BigInt random_bigint(uint64_t number_of_bits, bool top_two = false, bool bottom_odd = false);
    yabil::bigint::BigInt random_bigint(const yabil::bigint::BigInt &min, const yabil::bigint::BigInt &max);
    bigint::BigInt random_prime(uint64_t number_of_bits);

    bigint::bigint_base_t random_digit(std::uniform_int_distribution<yabil::bigint::bigint_base_t> distribution =
                                           std::uniform_int_distribution<yabil::bigint::bigint_base_t>{});

private:
    yabil::bigint::BigInt probable_prime(uint64_t number_of_bits);

    bool miller_rabin_round(const yabil::bigint::BigInt &prime_candidate,
                            const yabil::bigint::BigInt &prime_candidate_minus_one,
                            const yabil::bigint::BigInt &odd_component, int two_power_divisor);

    bool miller_rabin_test(const yabil::bigint::BigInt &prime_candidate);
};

yabil::bigint::BigInt RandomEngine::Impl::random_bigint(uint64_t number_of_bits, bool top_two, bool bottom_odd)
{
    constexpr std::size_t chunk_size_bits = sizeof(yabil::bigint::bigint_base_t) * 8;
    const std::size_t bigint_chunks_count = number_of_bits / chunk_size_bits;

    std::vector<yabil::bigint::bigint_base_t> raw_bigint_data;
    raw_bigint_data.reserve(bigint_chunks_count + 1);

    for (std::size_t i = 0; i < bigint_chunks_count; i++)
    {
        raw_bigint_data.push_back(uniform_dist(generator));
    }

    const std::size_t bigint_remaining_bits = number_of_bits % chunk_size_bits;
    if (bigint_remaining_bits)
    {
        raw_bigint_data.push_back(uniform_dist(generator) >> (chunk_size_bits - bigint_remaining_bits));
    }

    assert(!raw_bigint_data.empty());

    if (bottom_odd)
    {
        raw_bigint_data.front() |= 1;
    }

    if (top_two)
    {
        raw_bigint_data.back() |= static_cast<yabil::bigint::bigint_base_t>(1)
                                  << (chunk_size_bits - bigint_remaining_bits - 1);
    }

    return yabil::bigint::BigInt(std::move(raw_bigint_data));
}

yabil::bigint::BigInt RandomEngine::Impl::random_bigint(const yabil::bigint::BigInt &min,
                                                        const yabil::bigint::BigInt &max)
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

bigint::BigInt RandomEngine::Impl::random_prime(uint64_t number_of_bits)
{
    if (number_of_bits <= 2)
    {
        throw std::invalid_argument("There is no prime of 2 bits size");
    }

    // Arbitrary limit to avoid endless iteration over loop in worst cases
    constexpr unsigned max_number_of_trials = 128000;

    for (unsigned i = 0; i < max_number_of_trials; ++i)
    {
        bigint::BigInt prime_candidate = probable_prime(number_of_bits);
        if (miller_rabin_test(prime_candidate))
        {
            return prime_candidate;
        }
    }

    throw std::runtime_error("Cannot generate prime in: " + std::to_string(max_number_of_trials) + " steps");
}

bigint::bigint_base_t RandomEngine::Impl::random_digit(
    std::uniform_int_distribution<yabil::bigint::bigint_base_t> distribution)
{
    return distribution(generator);
}

yabil::bigint::BigInt RandomEngine::Impl::probable_prime(uint64_t number_of_bits)
{
    int trial_division_count = trial_divisions(number_of_bits);

    while (true)
    {
        yabil::bigint::BigInt prime_candidate = random_bigint(number_of_bits, true, true);
        bool good_candidate = true;

        for (int i = 1; i < trial_division_count; ++i)
        {
            const auto mod = prime_candidate % primes[i];
            if (mod == 0)
            {
                good_candidate = prime_candidate.is<uint64_t>()
                                     ? (prime_candidate.to<uint64_t>() == static_cast<uint64_t>(primes[i]))
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

bool RandomEngine::Impl::miller_rabin_round(const yabil::bigint::BigInt &prime_candidate,
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

bool RandomEngine::Impl::miller_rabin_test(const yabil::bigint::BigInt &prime_candidate)
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

RandomEngine::RandomEngine(const std::size_t seed) : impl(std::make_unique<Impl>(seed)) {}

RandomEngine::~RandomEngine() = default;

yabil::bigint::BigInt RandomEngine::random_bigint(uint64_t number_of_bits, bool top_two, bool bottom_odd)
{
    return impl->random_bigint(number_of_bits, top_two, bottom_odd);
}

yabil::bigint::BigInt RandomEngine::random_bigint(const yabil::bigint::BigInt &min, const yabil::bigint::BigInt &max)
{
    return impl->random_bigint(min, max);
}

bigint::BigInt RandomEngine::random_prime(uint64_t number_of_bits)
{
    return impl->random_prime(number_of_bits);
}

bigint::bigint_base_t RandomEngine::random_digit(
    std::uniform_int_distribution<yabil::bigint::bigint_base_t> distribution)
{
    return impl->random_digit(distribution);
}

}  // namespace yabil::random
