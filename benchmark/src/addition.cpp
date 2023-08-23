#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/Parallel.h>
#include <yabil/crypto/Random.h>

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
#include <string>
#include <thread>

#include "benchmark_utils.h"  // NOLINT

namespace
{

class Addition : public BaseBigIntBenchmark
{
};

BENCHMARK_DEFINE_F(Addition, YABIL)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    yabil::bigint::BigInt a{a_data};
    yabil::bigint::BigInt b{b_data};

    for (auto _ : state)
    {
        auto c = a + b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(Addition, YABIL_parallel)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    yabil::bigint::BigInt a{a_data};
    yabil::bigint::BigInt b{b_data};

    for (auto _ : state)
    {
        auto c = yabil::bigint::parallel::add(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(Addition, GMP)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    mpz_t a, b, c;
    convertTo_(a, a_data);
    convertTo_(b, b_data);

    for (auto _ : state)
    {
        mpz_init(c);
        mpz_add(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        mpz_clear(c);
    }
}

BENCHMARK_DEFINE_F(Addition, BIGINT_mattmccutchen)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    BigInteger a, b;
    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    for (auto _ : state)
    {
        auto c = a + b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(Addition, boost)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    boost::multiprecision::cpp_int a;
    boost::multiprecision::cpp_int b;
    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    boost::multiprecision::cpp_int c;

    for (auto _ : state)
    {
        c = a + b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    static_cast<void>(c);
}

BENCHMARK_DEFINE_F(Addition, openssl)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    BIGNUM* a = BN_new();
    BIGNUM* b = BN_new();

    convertTo_(a, a_data);
    convertTo_(b, b_data);

    for (auto _ : state)
    {
        BIGNUM* c = BN_new();
        BN_add(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        BN_free(c);
    }

    BN_free(a);
    BN_free(b);
}

BENCHMARK_DEFINE_F(Addition, python)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    Py_Initialize();
    PyObject* a;
    PyObject* b;

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    PyObject* c = nullptr;

    for (auto _ : state)
    {
        c = PyNumber_Add(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        Py_DECREF(c);
    }

    Py_DECREF(a);
    Py_DECREF(b);
    Py_Finalize();
}

REGISTER_F(Addition, YABIL);
REGISTER_F(Addition, YABIL_parallel);
REGISTER_F(Addition, GMP);
REGISTER_F(Addition, BIGINT_mattmccutchen);
REGISTER_F(Addition, boost);
REGISTER_F(Addition, openssl);
REGISTER_F(Addition, python);

}  // namespace
