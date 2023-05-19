#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/Parallel.h>
#include <yabil/utils/ThreadPoolSingleton.h>

#include <algorithm>
#include <future>
#include <vector>

#include "Arithmetic.h"
#include "Thresholds.h"

namespace yabil::bigint::parallel
{

namespace
{

std::vector<bigint_base_t> parallel_add_unsigned(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    const auto min_s = std::min(a.size(), b.size());
    if (min_s < parallel_add_digits)
    {
        return BigInt::plain_add(a, b);
    }

    auto &thread_pool = utils::ThreadPoolSingleton::instance();
    auto concurrency = thread_pool.thread_count();
    if (min_s < concurrency)
    {
        concurrency = min_s;
    }

    const auto chunk_size = min_s / concurrency;

    std::vector<std::future<std::vector<bigint_base_t>>> partial_results;
    partial_results.reserve(concurrency);

    for (int i = 0; i < static_cast<int>(concurrency); ++i)
    {
        partial_results.push_back(thread_pool.submit(
            [&, i]()
            {
                return BigInt::plain_add(
                    {a.begin() + static_cast<int>(i * chunk_size), a.begin() + static_cast<int>((i + 1) * chunk_size)},
                    {b.begin() + static_cast<int>(i * chunk_size), b.begin() + static_cast<int>((i + 1) * chunk_size)});
            }));
    }

    auto last_part = thread_pool.submit(
        [&]()
        {
            return BigInt::plain_add({a.begin() + static_cast<int>(concurrency * chunk_size), a.end()},
                                     {b.begin() + static_cast<int>(concurrency * chunk_size), b.end()});
        });

    std::vector<bigint_base_t> result(std::max(a.size(), b.size()) + 1);
    bigint_base_t carry = 0;

    int chunk_index = 0;
    for (auto &chunk : partial_results)
    {
        auto part_data = chunk.get();
        if (carry)
        {
            BigInt::increment_unsigned(part_data);
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
        BigInt::increment_unsigned(final_part_data);
    }

    std::copy(final_part_data.cbegin(), final_part_data.cend(), result.begin() + chunk_index);
    return result;
}

}  // namespace

BigInt parallel_add(const BigInt &a, const BigInt &b)
{
    if (a.get_sign() == b.get_sign())
    {
        return BigInt(parallel_add_unsigned(a.raw_data(), b.raw_data()), a.get_sign());
    }

    const auto [greater, lower] = get_greater_lower(a, b);
    const Sign new_sign = ((greater == &a) == (a.get_sign() == Sign::Plus)) ? Sign::Plus : Sign::Minus;
    return BigInt(BigInt::plain_sub(greater->raw_data(), lower->raw_data()), new_sign);
}

}  // namespace yabil::bigint::parallel
