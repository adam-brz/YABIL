#include <benchmark/benchmark.h>
#include <yabil/bigint/BigInt.h>
#include <gmp.h>

#include <thread>

#include "utils.h"

using namespace yabil::bigint;

static void addition_YABIL(benchmark::State& state) {
  BigInt a{generate_random_number_string(state.range(0))};
  BigInt b{generate_random_number_string(state.range(0))};
  for (auto _ : state) {
    auto c = a + b;
    benchmark::DoNotOptimize(c);
    benchmark::ClobberMemory();
  }
}

static void addition_GMP(benchmark::State& state) {
  const auto N1 = generate_random_number_string(state.range(0));
  const auto N2 = generate_random_number_string(state.range(0));

  mpz_t a, b, c;
  mpz_init(a);
  mpz_init(b);
  mpz_init(c);
  mpz_set_str(a, N1.c_str(), 10);
  mpz_set_str(b, N2.c_str(), 10);

  for (auto _ : state) {
    mpz_add(c, a, b);
    benchmark::DoNotOptimize(c);
    benchmark::ClobberMemory();
  }

  mpz_clear(a);
  mpz_clear(b);
  mpz_clear(c);
}

BENCHMARK(addition_YABIL)->DenseRange(1, 100, 10)->Threads(std::thread::hardware_concurrency());
BENCHMARK(addition_GMP)->DenseRange(1, 100, 10)->Threads(std::thread::hardware_concurrency());
