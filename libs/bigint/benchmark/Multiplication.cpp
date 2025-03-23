#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/test_utils/BenchmarkUtils.h>

namespace yabil::bigint
{

class MultiplicationBenchmark : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(MultiplicationBenchmark, TwoPositiveNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{test_utils::random_digits<bigint_base_t>(number_size)};
    const BigInt b{test_utils::random_digits<bigint_base_t>(number_size)};

    for (auto _ : st)
    {
        auto result = a * b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(MultiplicationBenchmark, InPlaceTwoPositiveNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{test_utils::random_digits<bigint_base_t>(number_size)};
    const BigInt b{test_utils::random_digits<bigint_base_t>(number_size)};

    for (auto _ : st)
    {
        st.PauseTiming();
        BigInt result = a;
        st.ResumeTiming();

        result *= b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(MultiplicationBenchmark, PositiveAndNegativeNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{test_utils::random_digits<bigint_base_t>(number_size), Sign::Minus};
    const BigInt b{test_utils::random_digits<bigint_base_t>(number_size)};

    for (auto _ : st)
    {
        auto result = a * b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

#ifdef NDEBUG
static constexpr int max_range = 1e6;
#else
static constexpr int max_range = 1e4;
#endif

BENCHMARK_REGISTER_F(MultiplicationBenchmark, TwoPositiveNumbers)->Range(1, max_range);
BENCHMARK_REGISTER_F(MultiplicationBenchmark, InPlaceTwoPositiveNumbers)->Range(1, max_range);
BENCHMARK_REGISTER_F(MultiplicationBenchmark, PositiveAndNegativeNumbers)->Range(1, max_range);

}  // namespace yabil::bigint
