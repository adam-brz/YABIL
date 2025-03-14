
#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/test_utils/BenchmarkUtils.h>

namespace yabil::bigint
{

class StrConversionBenchmark : public benchmark::Fixture
{
};

BENCHMARK_DEFINE_F(StrConversionBenchmark, ToString)(benchmark::State &st)
{
    const auto number_size = st.range();
    const auto base = st.range(1);
    const BigInt a = BigInt{test_utils::random_digits<bigint_base_t>(number_size)} + BigInt{1};

    for (auto _ : st)
    {
        auto result = a.to_str(base);
        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_DEFINE_F(StrConversionBenchmark, FromString)(benchmark::State &st)
{
    const auto number_size = st.range();
    const auto base = st.range(1);

    for (auto _ : st)
    {
        BigInt result =
            BigInt{test_utils::random_str_digits(number_size, static_cast<int>(base)), static_cast<unsigned>(base)};

        benchmark::DoNotOptimize(result);
        benchmark::ClobberMemory();
    }
}

BENCHMARK_REGISTER_F(StrConversionBenchmark, ToString)
    ->ArgsProduct({benchmark::CreateRange(1, 256, /*multi=*/2), {2, 3, 4, 8, 10, 16}});

BENCHMARK_REGISTER_F(StrConversionBenchmark, FromString)
    ->ArgsProduct({benchmark::CreateRange(1, 1024, /*multi=*/2), {2, 3, 4, 8, 10, 16}});

}  // namespace yabil::bigint
