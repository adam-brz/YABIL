#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
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

// FLINT
#include <fmpz.h>

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

    yabil::bigint::BigInt a;
    yabil::bigint::BigInt b;

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

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

    yabil::bigint::BigInt a;
    yabil::bigint::BigInt b;

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    for (auto _ : state)
    {
        auto c = yabil::bigint::parallel::add(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(Addition, YABIL_parallel_thread)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const int thread_count = static_cast<int>(state.range(1));
    const auto [a_data, b_data] = generate_test_numbers(size);

    yabil::bigint::BigInt a;
    yabil::bigint::BigInt b;

    yabil::bigint::BigIntGlobalConfig::instance().set_thread_count(thread_count);

    convertTo_(&a, a_data);
    convertTo_(&b, b_data);

    for (auto _ : state)
    {
        auto c = yabil::bigint::parallel::add(a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }

    yabil::bigint::BigIntGlobalConfig::instance().set_thread_count(11);
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

BENCHMARK_DEFINE_F(Addition, FLINT)(benchmark::State& state)
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
        fmpz_add(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        fmpz_clear(c);
    }
}

REGISTER_F(Addition, YABIL);
REGISTER_F(Addition, YABIL_parallel)->UseRealTime();
BENCHMARK_REGISTER_F(Addition, YABIL_parallel_thread)
    ->UseRealTime()
    ->ArgsProduct({benchmark::CreateDenseRange(64, BaseBigIntBenchmark::number_max_size_digits,
                                               BaseBigIntBenchmark::step_size),
                   {1, 2, 3, 5, 7, 9}});

REGISTER_F(Addition, GMP);
REGISTER_F(Addition, boost);
REGISTER_F(Addition, openssl);
REGISTER_F(Addition, python);
REGISTER_F(Addition, FLINT);

// ----------
// Perform addition for large inputs

constexpr int extended_range_start = 0;
constexpr int extended_range_stop = 3'000'000;
constexpr int extended_range_step = extended_range_stop / BaseBigIntBenchmark::step_size;

BENCHMARK_REGISTER_F(Addition, YABIL)
    ->Name("Addition/YABIL_big")
    ->DenseRange(extended_range_start, extended_range_stop, extended_range_step);

BENCHMARK_REGISTER_F(Addition, YABIL_parallel_thread)
    ->Name("Addition/YABIL_big_parallel")
    ->UseRealTime()
    ->ArgsProduct({benchmark::CreateDenseRange(extended_range_start, extended_range_stop, extended_range_step), {5}});

}  // namespace
