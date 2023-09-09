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

// FLINT
#include <fmpz.h>

// Utils
#include <thread>

#include "benchmark_utils.h"  // NOLINT

namespace
{

class Subtraction : public BaseBigIntBenchmark
{
};

BENCHMARK_DEFINE_F(Subtraction, YABIL)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    yabil::bigint::BigInt a;
    yabil::bigint::BigInt b;

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    for (auto _ : state)
    {
        auto c = a - b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(Subtraction, GMP)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    mpz_t a, b, c;
    convertTo_(a, a_data);
    convertTo_(b, b_data);

    for (auto _ : state)
    {
        mpz_init(c);
        mpz_sub(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        mpz_clear(c);
    }
}

BENCHMARK_DEFINE_F(Subtraction, boost)(benchmark::State& state)
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
        c = a - b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    static_cast<void>(c);
}

BENCHMARK_DEFINE_F(Subtraction, openssl)(benchmark::State& state)
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
        BN_sub(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        BN_free(c);
    }

    BN_free(a);
    BN_free(b);
}

BENCHMARK_DEFINE_F(Subtraction, python)(benchmark::State& state)
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
        c = PyNumber_Subtract(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        Py_DECREF(c);
    }

    Py_DECREF(a);
    Py_DECREF(b);
    Py_Finalize();
}

BENCHMARK_DEFINE_F(Subtraction, FLINT)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_data, b_data] = generate_test_numbers(size);

    fmpz_t a, b;
    convertTo_(a, a_data);
    convertTo_(b, b_data);

    PyObject* c = nullptr;

    for (auto _ : state)
    {
        fmpz_t c;
        fmpz_init(c);
        fmpz_sub(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        fmpz_clear(c);
    }
}

REGISTER_F(Subtraction, YABIL);
REGISTER_F(Subtraction, GMP);
REGISTER_F(Subtraction, boost);
REGISTER_F(Subtraction, openssl);
REGISTER_F(Subtraction, python);
REGISTER_F(Subtraction, FLINT);

// ----------
// Perform subtraction for large inputs

constexpr int extended_range_start = 0;
constexpr int extended_range_stop = 20'000'000;
constexpr int extended_range_step = extended_range_stop / BaseBigIntBenchmark::number_of_probes;

BENCHMARK_REGISTER_F(Subtraction, YABIL)
    ->Name("Subtraction/YABIL_big")
    ->DenseRange(extended_range_start, extended_range_stop, extended_range_step);

BENCHMARK_REGISTER_F(Subtraction, GMP)
    ->Name("Subtraction/GMP_big")
    ->DenseRange(extended_range_start, extended_range_stop, extended_range_step);

BENCHMARK_REGISTER_F(Subtraction, boost)
    ->Name("Subtraction/boost_big")
    ->DenseRange(extended_range_start, extended_range_stop, extended_range_step);

}  // namespace
