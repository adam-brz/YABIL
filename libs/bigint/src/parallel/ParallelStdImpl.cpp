#include <yabil/bigint/BigInt.h>
#include <yabil/utils/IterUtils.h>
#include <yabil/utils/ThreadPoolSingleton.h>

#include <algorithm>
#include <future>
#include <vector>

#include "Arithmetic.h"
#include "ParallelImpl.h"
#include "Thresholds.h"

namespace yabil::bigint::parallel
{

std::size_t get_thread_count()
{
    return utils::ThreadPoolSingleton::instance().thread_count();
}

void set_thread_count(std::size_t thread_count)
{
    utils::ThreadPoolSingleton::instance().resize(thread_count);
}

std::vector<bigint_base_t> parallel_add_unsigned(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    const auto min_s = std::min(a.size(), b.size());
    if (min_s < thresholds::parallel_add_digits)
    {
        return plain_add(a, b);
    }

    auto &thread_pool = utils::ThreadPoolSingleton::instance();
    const auto concurrency = std::min(min_s, thread_pool.thread_count());
    const auto chunk_size = min_s / concurrency;

    std::vector<std::future<std::vector<bigint_base_t>>> partial_results;
    partial_results.reserve(concurrency);

    for (int i = 0; i < static_cast<int>(concurrency); ++i)
    {
        partial_results.push_back(thread_pool.submit(
            [&, i]() {
                return plain_add({&a[i * chunk_size], chunk_size}, {&b[i * chunk_size], chunk_size});
            }));
    }

    auto last_part = thread_pool.submit(
        [&]()
        {
            return plain_add(utils::make_span(a.begin() + static_cast<int>(concurrency * chunk_size), a.end()),
                             utils::make_span(b.begin() + static_cast<int>(concurrency * chunk_size), b.end()));
        });

    std::vector<bigint_base_t> result(std::max(a.size(), b.size()) + 1);
    bigint_base_t carry = 0;

    int chunk_index = 0;
    for (auto &chunk : partial_results)
    {
        auto part_data = chunk.get();
        if (carry)
        {
            increment_unsigned(part_data);
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
        increment_unsigned(final_part_data);
    }

    std::copy(final_part_data.cbegin(), final_part_data.cend(), result.begin() + chunk_index);
    return result;
}

std::vector<bigint_base_t> parallel_karatsuba(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    if (a.size() < thresholds::karatsuba_threshold_digits || b.size() < thresholds::karatsuba_threshold_digits)
    {
        return mul_basecase(a, b);
    }

    const int m2 = static_cast<int>(std::max(a.size(), b.size()) / 2);

    const std::span<bigint_base_t const> low1 = utils::make_span(a.begin(), utils::safe_advance(a.begin(), m2, a));
    const std::span<bigint_base_t const> high1 = utils::make_span(utils::safe_advance(a.begin(), m2, a), a.end());

    const std::span<bigint_base_t const> low2 = utils::make_span(b.begin(), utils::safe_advance(b.begin(), m2, b));
    const std::span<bigint_base_t const> high2 = utils::make_span(utils::safe_advance(b.begin(), m2, b), b.end());

    auto &thread_pool = utils::ThreadPoolSingleton::instance();

    auto w_z0 = thread_pool.run_task([&]() { return parallel_karatsuba(low1, low2); });
    auto w_z1 = thread_pool.run_task(
        [&]()
        {
            const auto lh1 = plain_add(low1, high1);
            const auto lh2 = plain_add(low2, high2);
            return parallel_karatsuba(lh1, lh2);
        });
    auto w_z2 = thread_pool.run_task([&]() { return parallel_karatsuba(high1, high2); });

    const auto z0 = BigInt(w_z0.get());
    const auto z1 = BigInt(w_z1.get());
    const auto z2 = BigInt(w_z2.get());

    constexpr auto digit_bit_size = std::numeric_limits<bigint_base_t>::digits;
    const auto result = (z2 << (m2 * 2UL * digit_bit_size)) + ((z1 - z2 - z0) << (m2 * digit_bit_size)) + z0;
    return result.raw_data();
}

}  // namespace yabil::bigint::parallel
