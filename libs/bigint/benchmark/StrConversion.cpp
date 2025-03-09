
#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>

#include "BenchmarkUtils.h"

namespace yabil::bigint
{

class StrConversionBenchmark : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(StrConversionBenchmark, ConversionToString)(benchmark::State &st)
{
    const auto number_size = st.range();
    const auto base = st.range(1);
    const BigInt a = BigInt{benchmark_utils::random_digits(number_size)} + BigInt{1};

    for (auto _ : st)
    {
        auto result = a.to_str(base);
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_REGISTER_F(StrConversionBenchmark, ConversionToString)
    ->ArgsProduct({benchmark::CreateRange(1, 256, /*multi=*/2), {2, 3, 4, 8, 10, 16}});

}  // namespace yabil::bigint
