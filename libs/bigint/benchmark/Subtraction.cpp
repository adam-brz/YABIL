#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>

#include "BenchmarkUtils.h"

namespace yabil::bigint
{

class SubtractionBenchmark : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(SubtractionBenchmark, TwoPositiveNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        auto result = a - b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(SubtractionBenchmark, InPlaceTwoPositiveNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size)};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        st.PauseTiming();
        BigInt result = a;
        st.ResumeTiming();

        result -= b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(SubtractionBenchmark, PositiveAndNegativeNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{benchmark_utils::random_digits(number_size), Sign::Minus};
    const BigInt b{benchmark_utils::random_digits(number_size)};

    for (auto _ : st)
    {
        auto result = a - b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_REGISTER_F(SubtractionBenchmark, TwoPositiveNumbers)->Range(1, 1e8);
BENCHMARK_REGISTER_F(SubtractionBenchmark, InPlaceTwoPositiveNumbers)->Range(1, 1e8);
BENCHMARK_REGISTER_F(SubtractionBenchmark, PositiveAndNegativeNumbers)->Range(1, 1e8);

}  // namespace yabil::bigint
