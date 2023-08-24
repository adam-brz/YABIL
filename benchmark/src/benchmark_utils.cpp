#include "benchmark_utils.h"  // NOLINT

#include <yabil/bigint/BigInt.h>

#include <random>
#include <span>
#include <string>
#include <vector>

// Boost
#include <boost/multiprecision/cpp_int.hpp>

// GMP
#include <gmp.h>

// OpenSSL
#include <openssl/bn.h>

// CPython
#include <Python.h>

// BigInt https://mattmccutchen.net/bigint
#include <BigInteger.hh>
#include <BigIntegerUtils.hh>

std::vector<uint64_t> random_digits(uint64_t number_of_digits)
{
    constexpr int seed = 17;
    static std::mt19937 gen(seed);
    static std::uniform_int_distribution<uint64_t> dist(1, uint64_t(-1));

    std::vector<uint64_t> generated;
    generated.reserve(number_of_digits);

    for (std::size_t i = 0; i < number_of_digits; i++)
    {
        generated.push_back(dist(gen));
    }

    generated.back() |= static_cast<uint64_t>(1) << (sizeof(uint64_t) * 8 - 1);
    return generated;
}

std::pair<std::span<const uint64_t>, std::span<const uint64_t>> BaseBigIntBenchmark::generate_test_numbers(int size)
{
    constexpr auto digit_bit_size = sizeof(uint64_t) * 8;
    static const std::vector<uint64_t> a = random_digits(number_max_size_digits / digit_bit_size);
    static const std::vector<uint64_t> b = random_digits(number_max_size_digits / digit_bit_size);
    const auto number_of_digits = size / digit_bit_size;
    return std::make_pair(std::span(a.data(), number_of_digits), std::span(b.data(), number_of_digits));
}

template <>
void convertTo_(yabil::bigint::BigInt* output, std::span<const uint64_t> digits)
{
    yabil::bigint::BigInt converted(std::span(reinterpret_cast<const yabil::bigint::bigint_base_t*>(digits.data()),
                                              digits.size() * sizeof(uint64_t) / sizeof(yabil::bigint::bigint_base_t)));
    *output = std::move(converted);
}

template <>
void convertTo_(mpz_t output, std::span<const uint64_t> digits)
{
    output->_mp_alloc = static_cast<int>(digits.size());
    output->_mp_size = static_cast<int>(digits.size());
    output->_mp_d = const_cast<mp_limb_t*>(digits.data());
}

template <>
void convertTo_(BigInteger* output, std::span<const uint64_t> digits)
{
    BigInteger converted(const_cast<uint64_t*>(digits.data()), digits.size(), BigInteger::positive);
    *output = std::move(converted);
}

template <>
void convertTo_(boost::multiprecision::cpp_int* output, std::span<const uint64_t> digits)
{
    boost::multiprecision::import_bits(*output, digits.begin(), digits.end());
}

template <>
void convertTo_(BIGNUM* output, std::span<const uint64_t> digits)
{
    BN_bin2bn(reinterpret_cast<uint8_t*>(const_cast<uint64_t*>(digits.data())), digits.size() * 8, output);
}

template <>
void convertTo_(PyObject** output, std::span<const uint64_t> digits)
{
    *output = _PyLong_FromByteArray(reinterpret_cast<const uint8_t*>(digits.data()), digits.size() * 8, 1, 0);
}
