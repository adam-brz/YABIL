#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/test_utils/BenchmarkUtils.h>

namespace yabil::bigint
{

class AdditionBenchmark : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(AdditionBenchmark, TwoPositiveNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{test_utils::random_digits<bigint_base_t>(number_size)};
    const BigInt b{test_utils::random_digits<bigint_base_t>(number_size)};

    for (auto _ : st)
    {
        auto result = a + b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(AdditionBenchmark, InPlaceTwoPositiveNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{test_utils::random_digits<bigint_base_t>(number_size)};
    const BigInt b{test_utils::random_digits<bigint_base_t>(number_size)};

    for (auto _ : st)
    {
        st.PauseTiming();
        BigInt result = a;
        st.ResumeTiming();

        result += b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(AdditionBenchmark, PositiveAndNegativeNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const BigInt a{test_utils::random_digits<bigint_base_t>(number_size), Sign::Minus};
    const BigInt b{test_utils::random_digits<bigint_base_t>(number_size)};

    for (auto _ : st)
    {
        auto result = a + b;
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_REGISTER_F(AdditionBenchmark, TwoPositiveNumbers)->Range(1, 1e8);
BENCHMARK_REGISTER_F(AdditionBenchmark, InPlaceTwoPositiveNumbers)->Range(1, 1e8);
BENCHMARK_REGISTER_F(AdditionBenchmark, PositiveAndNegativeNumbers)->Range(1, 1e8);

}  // namespace yabil::bigint
