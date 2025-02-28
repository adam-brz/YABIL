
#include <oneapi/tbb/global_control.h>
#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/parallel_invoke.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/impl/Arithmetic.h>
#include <yabil/parallel/ParallelGlobalConfig.h>
#include <yabil/utils/IterUtils.h>

#include <algorithm>
#include <thread>
#include <vector>

#include "ParallelImpl.h"

using namespace oneapi;

namespace yabil::parallel
{

std::size_t get_thread_count()
{
    return tbb::global_control::active_value(tbb::global_control::max_allowed_parallelism);
}

void set_thread_count(std::size_t thread_count)
{
    tbb::global_control{tbb::global_control::max_allowed_parallelism, thread_count};
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

    const std::size_t hc = std::thread::hardware_concurrency();
    const auto concurrency = std::min(min_s, hc > 0 ? hc : 1) - 1;
    const auto chunk_size = min_s / concurrency;

    std::vector<std::vector<bigint::bigint_base_t>> partial_results(concurrency + 1);

    tbb::parallel_invoke(
        [&]()
        {
            tbb::parallel_for(
                tbb::blocked_range<std::size_t>(0, concurrency, 1),
                [&](const tbb::blocked_range<std::size_t> &r)
                {
                    const auto i = r.begin();
                    partial_results[i] = bigint::impl::add_unsigned(
                        utils::make_span(a.begin() + static_cast<int>(i * chunk_size),
                                         a.begin() + static_cast<int>((i + 1) * chunk_size)),
                        utils::make_span(b.begin() + static_cast<int>(i * chunk_size),
                                         b.begin() + static_cast<int>((i + 1) * chunk_size)));
                },
                tbb::simple_partitioner());
        },
        [&]()
        {
            partial_results.back() = bigint::impl::add_unsigned(
                utils::make_span(a.begin() + static_cast<int>(concurrency * chunk_size), a.end()),
                utils::make_span(b.begin() + static_cast<int>(concurrency * chunk_size), b.end()));
        });

    std::vector<bigint::bigint_base_t> result(std::max(a.size(), b.size()) + 1);
    bigint::bigint_base_t carry = 0;

    int chunk_index = 0;
    for (auto it = partial_results.begin(); it != partial_results.end() - 1; ++it)
    {
        auto &part_data = *it;
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

    auto &final_part_data = partial_results.back();
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

    std::vector<bigint::bigint_base_t> w_z0, w_z1, w_z2;

    tbb::parallel_invoke([&]() { w_z0 = parallel_karatsuba(low1, low2); },
                         [&]()
                         {
                             const auto lh1 = bigint::impl::add_unsigned(low1, high1);
                             const auto lh2 = bigint::impl::add_unsigned(low2, high2);
                             w_z1 = parallel_karatsuba(lh1, lh2);
                         },
                         [&]() { w_z2 = parallel_karatsuba(high1, high2); });

    const auto z0 = bigint::BigInt(std::move(w_z0));
    const auto z1 = bigint::BigInt(std::move(w_z1));
    const auto z2 = bigint::BigInt(std::move(w_z2));

    const uint64_t shift_val = static_cast<uint64_t>(m2) * bigint::BigInt::digit_size_bits;
    auto result = (z2 << (shift_val * 2UL)) + ((z1 - z2 - z0) << shift_val) + z0;
    return result.raw_data();
}

}  // namespace yabil::parallel
