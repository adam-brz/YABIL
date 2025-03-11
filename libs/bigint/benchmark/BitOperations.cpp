#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>

#include "BenchmarkUtils.h"

namespace yabil::bigint
{

class BitOperationsBenchmark : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(BitOperationsBenchmark, And)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        auto result = a & b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, InPlaceAnd)(benchmark::State &st)
{
    const auto number_size = st.range();
    BigInt a{benchmark_utils::random_digits(number_size)};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        a &= b;
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, Or)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        auto result = a | b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, InPlaceOr)(benchmark::State &st)
{
    const auto number_size = st.range();
    BigInt a{benchmark_utils::random_digits(number_size)};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        a |= b;
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, Xor)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        auto result = a ^ b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, InPlaceXor)(benchmark::State &st)
{
    const auto number_size = st.range();
    BigInt a{benchmark_utils::random_digits(number_size)};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        a ^= b;
        benchmark::DoNotOptimize(a);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, Not)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        BigInt result = ~a;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, ShiftLeft)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const int shift = 53;

    for (auto _ : st)
    {
        auto result = a << shift;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, InPlaceShiftLeft)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const int shift = 53;

    for (auto _ : st)
    {
        st.PauseTiming();
        BigInt result = a;
        st.ResumeTiming();

        result <<= shift;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, ShiftRight)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const int shift = 53;

    for (auto _ : st)
    {
        auto result = a >> shift;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(BitOperationsBenchmark, InPlaceShiftRight)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const int shift = 53;

    for (auto _ : st)
    {
        st.PauseTiming();
        BigInt result = a;
        st.ResumeTiming();

        result >>= shift;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_REGISTER_F(BitOperationsBenchmark, And)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, InPlaceAnd)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, Or)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, InPlaceOr)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, Xor)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, InPlaceXor)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, Not)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, ShiftLeft)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, InPlaceShiftLeft)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, ShiftRight)->Range(1, 1e8);
BENCHMARK_REGISTER_F(BitOperationsBenchmark, InPlaceShiftRight)->Range(1, 1e8);

}  // namespace yabil::bigint
