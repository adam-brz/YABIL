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

#include "benchmark_utils.h"  // NOLINT

namespace
{

class Subtraction : public BaseBigIntBenchmark
{
};

BENCHMARK_DEFINE_F(Subtraction, YABIL)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_str, b_str] = generate_test_numbers(size);

    yabil::bigint::BigInt a{a_str};
    yabil::bigint::BigInt b{b_str};

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
    const auto [a_str, b_str] = generate_test_numbers(size);

    mpz_t a, b, c;
    mpz_init(a);
    mpz_init(b);
    mpz_set_str(a, a_str.c_str(), 10);
    mpz_set_str(b, b_str.c_str(), 10);

    for (auto _ : state)
    {
        mpz_init(c);
        mpz_sub(c, a, b);
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
        mpz_clear(c);
    }

    mpz_clear(a);
    mpz_clear(b);
}

BENCHMARK_DEFINE_F(Subtraction, BIGINT_mattmccutchen)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_str, b_str] = generate_test_numbers(size);

    BigInteger a = stringToBigInteger(a_str);
    BigInteger b = stringToBigInteger(b_str);

    for (auto _ : state)
    {
        auto c = a - b;
        benchmark::DoNotOptimize(c);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(Subtraction, boost)(benchmark::State& state)
{
    const int size = static_cast<int>(state.range(0));
    const auto [a_str, b_str] = generate_test_numbers(size);

    boost::multiprecision::cpp_int a{a_str};
    boost::multiprecision::cpp_int b{b_str};
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
    const auto [a_str, b_str] = generate_test_numbers(size);

    BIGNUM* a = BN_new();
    BIGNUM* b = BN_new();

    BN_dec2bn(&a, a_str.c_str());
    BN_dec2bn(&b, b_str.c_str());

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
    const auto [a_str, b_str] = generate_test_numbers(size);

    Py_Initialize();
    PyObject* a = PyLong_FromString(a_str.c_str(), NULL, 10);
    PyObject* b = PyLong_FromString(b_str.c_str(), NULL, 10);
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

REGISTER_F(Subtraction, YABIL);
REGISTER_F(Subtraction, GMP);
REGISTER_F(Subtraction, boost);
REGISTER_F(Subtraction, BIGINT_mattmccutchen);
REGISTER_F(Subtraction, openssl);
REGISTER_F(Subtraction, python);

}  // namespace
