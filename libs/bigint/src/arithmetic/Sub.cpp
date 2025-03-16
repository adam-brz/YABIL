#include <yabil/bigint/arithmetic/Sub.h>

#include <limits>
#include <span>
#include <vector>

#include "add_sub/AddSub.h"

namespace yabil::bigint::arithmetic
{

std::vector<bigint_base_t> sub_unsigned(const std::span<const bigint_base_t> &a,
                                        const std::span<const bigint_base_t> &b)
{
    std::vector<bigint_base_t> result_data(a.size());
    sub_arrays(a.data(), a.size(), b.data(), b.size(), result_data.data());
    return result_data;
}

std::vector<bigint_base_t> &decrement_unsigned(std::vector<bigint_base_t> &n)
{
    for (auto &digit : n)
    {
        --digit;
        if (digit != std::numeric_limits<bigint_base_t>::max())
        {
            break;
        }
    }
    return n;
}
}  // namespace yabil::bigint::arithmetic
