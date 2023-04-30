#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>

// Boost
#include <boost/multiprecision/cpp_int.hpp>

// GMP
#include <gmp.h>

// BigInt https://mattmccutchen.net/bigint
#include <BigInteger.hh>
#include <BigIntegerUtils.hh>

// OpenSSL
#include <openssl/bn.h>

// Utils
#include <thread>

#include "utils.h"  // NOLINT

namespace
{

static void division_YABIL(benchmark::State& state)  // NOLINT
{
    yabil::bigint::BigInt a{generate_random_number_string(state.range(0))};
    yabil::bigint::BigInt b{generate_random_number_string(state.range(0))};
    for (auto _ : state)
    {
        auto c = a / b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

static void division_GMP(benchmark::State& state)  // NOLINT
{
    const auto N1 = generate_random_number_string(state.range(0));
    const auto N2 = generate_random_number_string(state.range(0));

    mpz_t a, b, c;
    mpz_init(a);
    mpz_init(b);
    mpz_init(c);
    mpz_set_str(a, N1.c_str(), 10);
    mpz_set_str(b, N2.c_str(), 10);

    for (auto _ : state)
    {
        mpz_div(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(c);
}

static void division_BIGINT_mattmccutchen(benchmark::State& state)  // NOLINT
{
    const auto N1 = generate_random_number_string(state.range(0));
    const auto N2 = generate_random_number_string(state.range(0));

    BigInteger a = stringToBigInteger(N1);
    BigInteger b = stringToBigInteger(N2);
    for (auto _ : state)
    {
        auto c = a / b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

static void division_boost(benchmark::State& state)  // NOLINT
{
    const auto N1 = generate_random_number_string(state.range(0));
    const auto N2 = generate_random_number_string(state.range(0));

    boost::multiprecision::cpp_int a{N1};
    boost::multiprecision::cpp_int b{N2};
    for (auto _ : state)
    {
        auto c = a / b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

static void division_openssl(benchmark::State& state)  // NOLINT
{
    const auto N1 = generate_random_number_string(state.range(0));
    const auto N2 = generate_random_number_string(state.range(0));

    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* a = BN_new();
    BIGNUM* b = BN_new();
    BIGNUM* r1 = BN_new();
    BIGNUM* r2 = BN_new();

    BN_dec2bn(&a, N1.c_str());
    BN_dec2bn(&b, N2.c_str());

    for (auto _ : state)
    {
        BN_div(r1, r2, a, b, ctx);
        benchmark::DoNotOptimize(r1);
        benchmark::DoNotOptimize(r2);
        benchmark::ClobberMemory();
    }

    BN_free(a);
    BN_free(b);
    BN_free(r1);
    BN_free(r2);
    BN_CTX_free(ctx);
}

static const int hc = static_cast<int>(std::thread::hardware_concurrency());
static constexpr int stop = 100;
static constexpr int step = 10;

BENCHMARK(division_YABIL)->DenseRange(1, stop, step);
BENCHMARK(division_GMP)->DenseRange(1, stop, step);
BENCHMARK(division_boost)->DenseRange(1, stop, step);
BENCHMARK(division_openssl)->DenseRange(1, stop, step);
BENCHMARK(division_BIGINT_mattmccutchen)->DenseRange(1, stop, step);

}  // namespace
