#include <oneapi/tbb/parallel_for.h>
#include <oneapi/tbb/parallel_invoke.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/utils/MakeSpan.h>

#include <algorithm>
#include <thread>
#include <vector>

#include "Arithmetic.h"
#include "ParallelImpl.h"
#include "Thresholds.h"

using namespace oneapi;

namespace yabil::bigint::parallel
{

std::vector<bigint_base_t> parallel_add_unsigned(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    const auto min_s = std::min(a.size(), b.size());
    if (min_s < thresholds::parallel_add_digits)
    {
        return plain_add(a, b);
    }

    const std::size_t hc = std::thread::hardware_concurrency();
    const auto concurrency = std::min(min_s, hc > 0 ? hc : 1) - 1;
    const auto chunk_size = min_s / concurrency;

    std::vector<std::vector<bigint_base_t>> partial_results(concurrency + 1);

    tbb::parallel_invoke(
        [&]()
        {
            tbb::parallel_for(
                tbb::blocked_range<std::size_t>(0, concurrency, 1),
                [&](const tbb::blocked_range<std::size_t>& r)
                {
                    const auto i = r.begin();
                    partial_results[i] =
                        plain_add(utils::make_span(a.begin() + static_cast<int>(i * chunk_size),
                                                   a.begin() + static_cast<int>((i + 1) * chunk_size)),
                                  utils::make_span(b.begin() + static_cast<int>(i * chunk_size),
                                                   b.begin() + static_cast<int>((i + 1) * chunk_size)));
                },
                tbb::simple_partitioner());
        },
        [&]()
        {
            partial_results.back() =
                plain_add(utils::make_span(a.begin() + static_cast<int>(concurrency * chunk_size), a.end()),
                          utils::make_span(b.begin() + static_cast<int>(concurrency * chunk_size), b.end()));
        });

    std::vector<bigint_base_t> result(std::max(a.size(), b.size()) + 1);
    bigint_base_t carry = 0;

    int chunk_index = 0;
    for (auto it = partial_results.begin(); it != partial_results.end() - 1; ++it)
    {
        auto& part_data = *it;
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

    auto& final_part_data = partial_results.back();
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

    const std::span<bigint_base_t const> low1 = utils::make_span(a.begin(), a.begin() + m2);
    const std::span<bigint_base_t const> high1 = utils::make_span(a.begin() + m2, a.end());

    const std::span<bigint_base_t const> low2 = utils::make_span(b.begin(), b.begin() + m2);
    const std::span<bigint_base_t const> high2 = utils::make_span(b.begin() + m2, b.end());

    std::vector<bigint_base_t> w_z0, w_z1, w_z2;

    tbb::parallel_invoke([&]() { w_z0 = parallel_karatsuba(low1, low2); },
                         [&]()
                         {
                             const auto lh1 = plain_add(low1, high1);
                             const auto lh2 = plain_add(low2, high2);
                             w_z1 = parallel_karatsuba(lh1, lh2);
                         },
                         [&]() { w_z2 = parallel_karatsuba(high1, high2); });

    const auto z0 = BigInt(std::move(w_z0));
    const auto z1 = BigInt(std::move(w_z1));
    const auto z2 = BigInt(std::move(w_z2));

    const auto result =
        (z2 << (m2 * 2UL * sizeof(bigint_base_t) * 8UL)) + ((z1 - z2 - z0) << (m2 * sizeof(bigint_base_t) * 8UL)) + z0;
    return result.raw_data();
}

}  // namespace yabil::bigint::parallel
