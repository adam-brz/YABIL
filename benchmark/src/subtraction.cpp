#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>

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

// Utils
#include <thread>

#include "utils.h"  // NOLINT

namespace
{

static void subtraction_YABIL(benchmark::State& state)  // NOLINT
{
    yabil::bigint::BigInt a{generate_random_number_string(state.range(0))};
    yabil::bigint::BigInt b{generate_random_number_string(state.range(0))};
    for (auto _ : state)
    {
        auto c = a - b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

static void subtraction_GMP(benchmark::State& state)  // NOLINT
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
        mpz_sub(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(c);
}

static void subtraction_BIGINT_mattmccutchen(benchmark::State& state)  // NOLINT
{
    const auto N1 = generate_random_number_string(state.range(0));
    const auto N2 = generate_random_number_string(state.range(0));

    BigInteger a = stringToBigInteger(N1);
    BigInteger b = stringToBigInteger(N2);
    for (auto _ : state)
    {
        auto c = a - b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

static void subtraction_boost(benchmark::State& state)  // NOLINT
{
    const auto N1 = generate_random_number_string(state.range(0));
    const auto N2 = generate_random_number_string(state.range(0));

    boost::multiprecision::cpp_int a{N1};
    boost::multiprecision::cpp_int b{N2};
    boost::multiprecision::cpp_int c;

    for (auto _ : state)
    {
        c = a - b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    static_cast<void>(c);
}

static void subtraction_openssl(benchmark::State& state)  // NOLINT
{
    const auto N1 = generate_random_number_string(state.range(0));
    const auto N2 = generate_random_number_string(state.range(0));

    BIGNUM* a = BN_new();
    BIGNUM* b = BN_new();
    BIGNUM* c = BN_new();

    BN_dec2bn(&a, N1.c_str());
    BN_dec2bn(&b, N2.c_str());

    for (auto _ : state)
    {
        BN_sub(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    BN_free(a);
    BN_free(b);
    BN_free(c);
}

static void subtraction_python(benchmark::State& state)  // NOLINT
{
    const auto N1 = generate_random_number_string(state.range(0));
    const auto N2 = generate_random_number_string(state.range(0));

    Py_Initialize();
    PyObject* a = PyLong_FromString(N1.c_str(), NULL, 10);
    PyObject* b = PyLong_FromString(N2.c_str(), NULL, 10);
    PyObject* c = nullptr;

    for (auto _ : state)
    {
        c = PyNumber_Subtract(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        Py_DECREF(c);
    }

    Py_DECREF(a);
    Py_DECREF(b);
    Py_Finalize();
}

static constexpr uint64_t stop = 200000ULL;
static constexpr int step = stop / 100;

BENCHMARK(subtraction_YABIL)->Range(1, stop);
BENCHMARK(subtraction_GMP)->Range(1, stop);
BENCHMARK(subtraction_boost)->Range(1, stop);
BENCHMARK(subtraction_openssl)->Range(1, stop);
BENCHMARK(subtraction_python)->Range(1, stop);

}  // namespace
