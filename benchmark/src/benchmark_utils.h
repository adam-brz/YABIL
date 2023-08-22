#pragma once

#include <benchmark/benchmark.h>

#include <cstdint>
#include <string>
#include <utility>

std::string generate_random_number_string(int64_t digits);

class BaseBigIntBenchmark : public benchmark::Fixture
{
public:
    constexpr static int number_max_size_digits = 1'000'000;
    constexpr static int number_of_probes = 100;
    constexpr static int step_size = number_max_size_digits / number_of_probes;

public:
    static std::pair<std::string, std::string> generate_test_numbers(int size);
};

#define REGISTER_F(FixtureName, CaseName)       \
    BENCHMARK_REGISTER_F(FixtureName, CaseName) \
        ->DenseRange(1, BaseBigIntBenchmark::number_max_size_digits, BaseBigIntBenchmark::step_size)

// #define REGISTER_F(FixtureName, CaseName) BENCHMARK_REGISTER_F(FixtureName, CaseName)->Range(1, 1000)
