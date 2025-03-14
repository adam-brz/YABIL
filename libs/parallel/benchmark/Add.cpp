#include <benchmark/benchmark.h>
#include <yabil/parallel/Parallel.h>
#include <yabil/test_utils/BenchmarkUtils.h>

namespace yabil::parallel
{

class AdditionBenchmark : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(AdditionBenchmark, TwoPositiveNumbers)(benchmark::State &st)
{
    const auto number_size = st.range();
    const bigint::BigInt a{test_utils::random_digits<bigint::bigint_base_t>(number_size)};
    const bigint::BigInt b{test_utils::random_digits<bigint::bigint_base_t>(number_size)};

    for (auto _ : st)
    {
        auto result = parallel::add(a, b);
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_REGISTER_F(AdditionBenchmark, TwoPositiveNumbers)->Range(1, 1e8);

}  // namespace yabil::parallel
