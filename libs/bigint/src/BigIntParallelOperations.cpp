#include <yabil/bigint/BigInt.h>
#include <yabil/utils/ThreadPoolSingleton.h>

#include <algorithm>

#include "Thresholds.h"

namespace yabil::bigint
{

std::vector<bigint_base_t> BigInt::parallel_add_unsigned(const std::vector<bigint_base_t> &a,
                                                         const std::vector<bigint_base_t> &b)
{
    const auto min_s = std::min(a.size(), b.size());
    auto &thread_pool = utils::ThreadPoolSingleton::instance();
    auto concurrency = thread_pool.thread_count();

    if (min_s < parallel_add_digits)
    {
        return plain_add(a, b);
    }

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
                return plain_add(std::vector<bigint_base_t>(a.cbegin() + static_cast<int>(i * chunk_size),
                                                            a.cbegin() + static_cast<int>((i + 1) * chunk_size)),
                                 std::vector<bigint_base_t>(b.cbegin() + static_cast<int>(i * chunk_size),
                                                            b.cbegin() + static_cast<int>((i + 1) * chunk_size)));
            }));
    }

    auto last_part = thread_pool.submit(
        [&]()
        {
            return plain_add(
                std::vector<bigint_base_t>(a.cbegin() + static_cast<int>(concurrency * chunk_size), a.cend()),
                std::vector<bigint_base_t>(b.cbegin() + static_cast<int>(concurrency * chunk_size), b.cend()));
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
            carry = 1;
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

}  // namespace yabil::bigint
