#pragma once

#include <benchmark/benchmark.h>

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <utility>

class BaseBigIntBenchmark : public benchmark::Fixture
{
public:
    constexpr static int number_max_size_digits = 1'000'000;
    constexpr static int number_of_probes = 100;
    constexpr static int step_size = number_max_size_digits / number_of_probes;

public:
    static std::pair<std::span<const uint64_t>, std::span<const uint64_t>> generate_test_numbers(
        int size, std::optional<int> size2 = std::nullopt);
};

template <typename ResultType>
void convertTo_(ResultType* output, std::span<const uint64_t> digits);

#define REGISTER_F(FixtureName, CaseName)       \
    BENCHMARK_REGISTER_F(FixtureName, CaseName) \
        ->DenseRange(0, BaseBigIntBenchmark::number_max_size_digits, BaseBigIntBenchmark::step_size)

// #define REGISTER_F(FixtureName, CaseName) BENCHMARK_REGISTER_F(FixtureName, CaseName)->Range(1, 1000)
