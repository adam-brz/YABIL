#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/impl/Arithmetic.h>
#include <yabil/parallel/ParallelGlobalConfig.h>
#include <yabil/parallel/ThreadPoolSingleton.h>
#include <yabil/utils/IterUtils.h>

#include <algorithm>
#include <future>
#include <vector>

#include "ParallelImpl.h"

namespace yabil::parallel
{

std::size_t get_thread_count()
{
    return ThreadPoolSingleton::instance().thread_count();
}

void set_thread_count(std::size_t thread_count)
{
    ThreadPoolSingleton::instance().resize(thread_count);
}

std::vector<bigint::bigint_base_t> parallel_add_unsigned(const std::span<const bigint::bigint_base_t> &a,
                                                         const std::span<const bigint::bigint_base_t> &b)
{
    const auto min_s = std::min(a.size(), b.size());
    const auto &config = ParallelGlobalConfig::instance().config;

    if (min_s < config.parallel_add_threshold)
    {
        return bigint::impl::add_unsigned(a, b);
    }

    auto &thread_pool = ThreadPoolSingleton::instance();
    const auto concurrency = std::min(min_s, thread_pool.thread_count());
    const auto chunk_size = min_s / concurrency;

    std::vector<std::future<std::vector<bigint::bigint_base_t>>> partial_results;
    partial_results.reserve(concurrency);

    for (int i = 0; i < static_cast<int>(concurrency); ++i)
    {
        partial_results.push_back(thread_pool.submit(
            [&, i]()
            {
                return bigint::impl::add_unsigned({&a[i * chunk_size], chunk_size}, {&b[i * chunk_size], chunk_size});
            }));
    }

    auto last_part = thread_pool.submit(
        [&]()
        {
            return bigint::impl::add_unsigned(
                utils::make_span(a.begin() + static_cast<int>(concurrency * chunk_size), a.end()),
                utils::make_span(b.begin() + static_cast<int>(concurrency * chunk_size), b.end()));
        });

    std::vector<bigint::bigint_base_t> result(std::max(a.size(), b.size()) + 1);
    bigint::bigint_base_t carry = 0;

    int chunk_index = 0;
    for (auto &chunk : partial_results)
    {
        auto part_data = chunk.get();
        if (carry)
        {
            bigint::impl::increment_unsigned(part_data);
            carry = 0;
        }
        if (part_data.size() > chunk_size)
        {
            carry += part_data.back();
        }
        std::copy(part_data.cbegin(), part_data.cbegin() + static_cast<int>(chunk_size), result.begin() + chunk_index);
        chunk_index += static_cast<int>(chunk_size);
    }

    auto final_part_data = last_part.get();
    if (carry)
    {
        bigint::impl::increment_unsigned(final_part_data);
    }

    std::copy(final_part_data.cbegin(), final_part_data.cend(), result.begin() + chunk_index);
    return result;
}

std::vector<bigint::bigint_base_t> parallel_karatsuba(const std::span<const bigint::bigint_base_t> &a,
                                                      const std::span<const bigint::bigint_base_t> &b)
{
    const auto &parallel_config = ParallelGlobalConfig::instance().config;
    if (a.size() < parallel_config.parallel_mul_threshold || b.size() < parallel_config.parallel_mul_threshold)
    {
        return bigint::impl::mul_unsigned_karatsuba(a, b);
    }

    const auto &algorithms_config = bigint::BigIntGlobalConfig::instance().config;
    if (a.size() < algorithms_config.karatsuba_threshold || b.size() < algorithms_config.karatsuba_threshold)
    {
        return bigint::impl::mul_unsigned_basecase(a, b);
    }

    const int m2 = static_cast<int>(std::max(a.size(), b.size()) / 2);

    const std::span<const bigint::bigint_base_t> low1 =
        utils::make_span(a.begin(), utils::safe_advance(a.begin(), m2, a));
    const std::span<const bigint::bigint_base_t> high1 =
        utils::make_span(utils::safe_advance(a.begin(), m2, a), a.end());

    const std::span<const bigint::bigint_base_t> low2 =
        utils::make_span(b.begin(), utils::safe_advance(b.begin(), m2, b));
    const std::span<const bigint::bigint_base_t> high2 =
        utils::make_span(utils::safe_advance(b.begin(), m2, b), b.end());

    auto &thread_pool = ThreadPoolSingleton::instance();

    auto w_z0 = thread_pool.submit([&]() { return bigint::impl::mul_unsigned_karatsuba(low1, low2); });
    auto w_z1 = thread_pool.submit(
        [&]()
        {
            const auto lh1 = bigint::impl::add_unsigned(low1, high1);
            const auto lh2 = bigint::impl::add_unsigned(low2, high2);
            return bigint::impl::mul_unsigned_karatsuba(lh1, lh2);
        });
    auto w_z2 = thread_pool.submit([&]() { return bigint::impl::mul_unsigned_karatsuba(high1, high2); });

    const auto z0 = bigint::BigInt(w_z0.get());
    const auto z1 = bigint::BigInt(w_z1.get());
    const auto z2 = bigint::BigInt(w_z2.get());

    const uint64_t shift_val = static_cast<uint64_t>(m2) * bigint::BigInt::digit_size_bits;
    auto result = (z2 << (shift_val * 2UL)) + ((z1 - z2 - z0) << shift_val) + z0;
    return result.raw_data();
}

}  // namespace yabil::parallel
