#include <yabil/bigint/arithmetic/Add.h>

#include <span>
#include <vector>

#include "add_sub/AddSub.h"
#include "impl/Arithmetic.h"

namespace yabil::bigint::arithmetic
{

std::vector<bigint_base_t> add_unsigned(const std::span<const bigint_base_t> &a,
                                        const std::span<const bigint_base_t> &b)
{
    const auto [longer, shorter] = impl::get_longer_shorter(&a, &b);
    std::vector<bigint_base_t> result_data(longer->size() + 1);
    add_arrays(longer->data(), longer->size(), shorter->data(), shorter->size(), result_data.data());
    return result_data;
}

std::vector<bigint_base_t> &increment_unsigned(std::vector<bigint_base_t> &n)
{
    bigint_base_t carry = 1;
    for (auto &digit : n)
    {
        ++digit;
        if (digit != 0)
        {
            carry = 0;
            break;
        }
    }
    if (carry)
    {
        n.push_back(carry);
    }
    return n;
}

}  // namespace yabil::bigint::arithmetic
