#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/parallel/Parallel.h>
#include <yabil/test_utils/BenchmarkUtils.h>

namespace yabil::parallel
{

class MultiplicationBenchmark : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(MultiplicationBenchmark, TwoPositiveNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const bigint::BigInt a{test_utils::random_digits<bigint::bigint_base_t>(number_size)};
    const bigint::BigInt b{test_utils::random_digits<bigint::bigint_base_t>(number_size)};

    for (auto _ : st)
    {
        auto result = parallel::mul(a, b);
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_REGISTER_F(MultiplicationBenchmark, TwoPositiveNumbers)->Range(1, 1e6);

}  // namespace yabil::parallel
