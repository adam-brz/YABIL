#include <yabil/bigint/BigInt.h>
#include <yabil/utils/ThreadPoolSingleton.h>

#include <algorithm>

#include "Thresholds.h"

namespace yabil::bigint
{

BigInt BigInt::parallel_add_unsigned(const BigInt &b) const
{
    const auto min_s = std::min(raw_data().size(), b.raw_data().size());
    auto &thread_pool = utils::ThreadPoolSingleton::instance();
    auto concurrency = thread_pool.thread_count();

    if (min_s < parallel_add_digits)
    {
        return plain_add(b);
    }

    if (min_s < concurrency)
    {
        concurrency = min_s;
    }

    const auto &a_data = raw_data();
    const auto &b_data = b.raw_data();

    const auto chunk_size = min_s / concurrency;

    std::vector<std::future<BigInt>> partial_results;
    partial_results.reserve(concurrency);

    for (int i = 0; i < static_cast<int>(concurrency); ++i)
    {
        partial_results.push_back(thread_pool.submit(
            [&, i]()
            {
                return BigInt(std::vector<bigint_base_t>(a_data.cbegin() + static_cast<int>(i * chunk_size),
                                                         a_data.cbegin() + static_cast<int>((i + 1) * chunk_size)))
                    .plain_add(
                        BigInt(std::vector<bigint_base_t>(b_data.cbegin() + static_cast<int>(i * chunk_size),
                                                          b_data.cbegin() + static_cast<int>((i + 1) * chunk_size))));
            }));
    }

    auto last_part = thread_pool.submit(
        [&]()
        {
            return BigInt(std::vector<bigint_base_t>(a_data.cbegin() + static_cast<int>(concurrency * chunk_size),
                                                     a_data.cend()))
                .plain_add(BigInt(std::vector<bigint_base_t>(
                    b_data.cbegin() + static_cast<int>(concurrency * chunk_size), b_data.cend())));
        });

    std::vector<bigint_base_t> result(std::max(a_data.size(), b_data.size()) + 1);
    bigint_base_t carry = 0;

    int chunk_index = 0;
    for (auto &chunk : partial_results)
    {
        auto part_data = chunk.get();
        if (carry)
        {
            part_data.increment_unsigned();
            carry = 0;
        }
        if (part_data.raw_data().size() > chunk_size)
        {
            carry = 1;
        }
        std::copy(part_data.raw_data().cbegin(), part_data.raw_data().cbegin() + static_cast<int>(chunk_size),
                  result.begin() + chunk_index);
        chunk_index += static_cast<int>(chunk_size);
    }

    auto final_part_data = last_part.get();
    if (carry)
    {
        final_part_data.increment_unsigned();
    }

    std::copy(final_part_data.raw_data().cbegin(), final_part_data.raw_data().cend(), result.begin() + chunk_index);
    return BigInt(std::move(result), sign);
}

}  // namespace yabil::bigint
