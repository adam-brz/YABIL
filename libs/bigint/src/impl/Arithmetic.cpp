#include "impl/Arithmetic.h"

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/Sign.h>
#include <yabil/utils/IterUtils.h>
#include <yabil/utils/TypeUtils.h>

#include <algorithm>
#include <cassert>
#include <vector>

#include "add_sub/AddSubInterface.h"

namespace yabil::bigint::impl
{

void remove_trailing_zeros(std::vector<bigint_base_t> &data)
{
    const auto data_end = std::find_if(data.rbegin(), data.rend(), [](const auto &v) { return v != 0; }).base();
    data.resize(std::distance(data.begin(), data_end));
}

bool is_normalized_for_division(const BigInt &n)
{
    return n.get_bit(n.byte_size() * 8 - 1);
}

std::pair<const std::span<const bigint_base_t> *, const std::span<const bigint_base_t> *> get_longer_shorter(
    const std::span<const bigint_base_t> *a, const std::span<const bigint_base_t> *b)
{
    if (a->size() < b->size())
    {
        return std::make_pair(b, a);
    }
    return std::make_pair(a, b);
}

std::pair<const BigInt *, const BigInt *> get_longer_shorter(const BigInt &a, const BigInt &b)
{
    if (a.raw_data().size() < b.raw_data().size())
    {
        return std::make_pair(&b, &a);
    }
    return std::make_pair(&a, &b);
}

std::pair<const BigInt *, const BigInt *> get_greater_lower_unsigned(const BigInt &a, const BigInt &b)
{
    if (a.abs_lower(b))
    {
        return std::make_pair(&b, &a);
    }
    return std::make_pair(&a, &b);
}

bool abs_greater(const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    return abs_lower(b, a);
}

bool abs_lower(const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    return a.size() < b.size() ||
           (a.size() == b.size() && std::lexicographical_compare(a.rbegin(), a.rend(), b.rbegin(), b.rend()));
}

std::vector<bigint_base_t> &inplace_plain_add(std::vector<bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    assert(a.size() > 0 && a.back() == 0);
    add_arrays(a.data(), a.size(), b.data(), b.size(), a.data());
    impl::remove_trailing_zeros(a);
    return a;
}

std::pair<std::reference_wrapper<std::vector<bigint_base_t>>, Sign> inplace_plain_sub(
    std::vector<bigint_base_t> &a, const std::span<const bigint_base_t> &b, const Sign a_sign, const Sign b_sign)
{
    const std::span<const bigint_base_t> a_view{a};

    const auto *longer = &a_view;
    const auto *shorter = &b;

    Sign sign = a_sign;
    if (impl::abs_lower(a, b))
    {
        std::swap(longer, shorter);
        sign = b_sign;
    }

    assert(a.size() == longer->size());

    sub_arrays(longer->data(), longer->size(), shorter->data(), shorter->size(), a.data());
    impl::remove_trailing_zeros(a);
    return {std::ref(a), sign};
}

std::vector<bigint_base_t> shift_digits_left(const std::span<const bigint_base_t> &data, const int shift)
{
    std::vector<bigint_base_t> result(data.size() + shift);
    std::copy(data.begin(), data.end(), result.begin() + shift);
    return result;
}

}  // namespace yabil::bigint::impl
