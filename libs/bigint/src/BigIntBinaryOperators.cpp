#include <yabil/bigint/BigInt.h>

#include <algorithm>

#include "Arithmetic.h"
#include "SafeOperators.h"

namespace yabil::bigint
{

BigInt BigInt::operator&(const BigInt &other) const
{
    const std::size_t min_size = std::min(data.size(), other.data.size());

    std::vector<bigint_base_t> result_data;
    result_data.reserve(min_size);
    for (std::size_t i = 0; i < min_size; ++i)
    {
        result_data.push_back(data[i] & other.data[i]);
    }

    return BigInt(std::move(result_data),
                  (sign == Sign::Minus && other.sign == Sign::Minus) ? Sign::Minus : Sign::Plus);
}

BigInt BigInt::operator|(const BigInt &other) const
{
    const auto [longer, shorter] = get_longer_shorter(*this, other);
    std::vector<bigint_base_t> result_data;
    result_data.reserve(longer->data.size());

    std::size_t i;
    for (i = 0; i < shorter->data.size(); ++i)
    {
        result_data.push_back(longer->data[i] | shorter->data[i]);
    }
    for (; i < longer->data.size(); ++i)
    {
        result_data.push_back(longer->data[i]);
    }

    return BigInt(std::move(result_data),
                  (sign == Sign::Minus || other.sign == Sign::Minus) ? Sign::Minus : Sign::Plus);
}

BigInt BigInt::operator^(const BigInt &other) const
{
    const auto [longer, shorter] = get_longer_shorter(*this, other);
    std::vector<bigint_base_t> result_data;
    result_data.reserve(longer->data.size());

    std::size_t i;
    for (i = 0; i < shorter->data.size(); ++i)
    {
        result_data.push_back(longer->data[i] ^ shorter->data[i]);
    }
    for (; i < longer->data.size(); ++i)
    {
        result_data.push_back(longer->data[i]);
    }

    return BigInt(std::move(result_data), (sign != other.sign) ? Sign::Minus : Sign::Plus);
}

BigInt BigInt::operator<<(uint64_t shift) const
{
    const uint64_t new_items_count = shift / (sizeof(bigint_base_t) * 8);
    const uint64_t real_shift = shift % (sizeof(bigint_base_t) * 8);

    std::vector<bigint_base_t> shifted(new_items_count + data.size() + 1, 0);
    bigint_base_t shifted_val = 0;

    std::transform(data.cbegin(), data.cend(), shifted.begin() + static_cast<int>(new_items_count),
                   [real_shift, &shifted_val](const bigint_base_t &v)
                   {
                       const bigint_base_t transformed = (v << real_shift) | shifted_val;
                       shifted_val =
                           static_cast<bigint_base_t>((static_cast<double_width_t<bigint_base_t>>(v) << real_shift) >>
                                                      (sizeof(bigint_base_t) * 8));
                       return transformed;
                   });

    shifted.back() = shifted_val;
    return BigInt(std::move(shifted), sign);
}

BigInt BigInt::operator>>(uint64_t shift) const
{
    const uint64_t removed_items_count = shift / (sizeof(bigint_base_t) * 8);
    const uint64_t real_shift = shift % (sizeof(bigint_base_t) * 8);

    if (removed_items_count >= data.size())
    {
        return BigInt();
    }

    std::vector<bigint_base_t> shifted(data.size() - removed_items_count, 0);
    bigint_base_t shifted_val = 0;

    std::transform(
        data.crbegin(), data.crend() - static_cast<int>(removed_items_count), shifted.rbegin(),
        [real_shift, &shifted_val](const bigint_base_t &v)
        {
            const bigint_base_t transformed = (v >> real_shift) | shifted_val;
            shifted_val = static_cast<bigint_base_t>(
                (static_cast<double_width_t<bigint_base_t>>(v) << (sizeof(bigint_base_t) * 8)) >> real_shift);
            return transformed;
        });

    return BigInt(std::move(shifted), sign);
}

BigInt BigInt::operator~() const
{
    std::vector<bigint_base_t> result_data(data.size());
    std::transform(data.cbegin(), data.cend(), result_data.begin(), [](const auto &v) { return ~v; });
    return BigInt(std::move(result_data), (sign == Sign::Plus) ? Sign::Minus : Sign::Plus);
}

BigInt &BigInt::operator&=(const BigInt &other)
{
    data.resize(std::min(data.size(), other.data.size()));

    for (std::size_t i = 0; i < data.size(); ++i)
    {
        data[i] &= other.data[i];
    }

    sign = (sign == Sign::Minus && other.sign == Sign::Minus) ? Sign::Minus : Sign::Plus;
    normalize();
    return *this;
}

BigInt &BigInt::operator|=(const BigInt &other)
{
    data.resize(std::max(data.size(), other.data.size()));

    for (std::size_t i = 0; i < other.data.size(); ++i)
    {
        data[i] |= other.data[i];
    }

    sign = (sign == Sign::Minus || other.sign == Sign::Minus) ? Sign::Minus : Sign::Plus;
    normalize();
    return *this;
}

BigInt &BigInt::operator^=(const BigInt &other)
{
    data.resize(std::max(data.size(), other.data.size()));

    for (std::size_t i = 0; i < other.data.size(); ++i)
    {
        data[i] ^= other.data[i];
    }

    sign = (sign != other.sign) ? Sign::Minus : Sign::Plus;
    normalize();
    return *this;
}

BigInt &BigInt::operator<<=(uint64_t shift)
{
    const uint64_t new_items_count = shift / (sizeof(bigint_base_t) * 8);
    const uint64_t real_shift = shift % (sizeof(bigint_base_t) * 8);

    data.resize(data.size() + new_items_count + 1);
    bigint_base_t shifted_val = 0;

    std::transform(data.cbegin(), data.cend() - static_cast<int>(new_items_count) - 1,
                   data.begin() + static_cast<int>(new_items_count),
                   [real_shift, &shifted_val](const bigint_base_t &v)
                   {
                       const bigint_base_t transformed = (v << real_shift) | shifted_val;
                       shifted_val =
                           static_cast<bigint_base_t>((static_cast<double_width_t<bigint_base_t>>(v) << real_shift) >>
                                                      (sizeof(bigint_base_t) * 8));
                       return transformed;
                   });

    if (new_items_count > 0)
    {
        std::fill(data.begin(), data.begin() + static_cast<int>(new_items_count), 0);
    }

    data.back() = shifted_val;
    normalize();

    return *this;
}

BigInt &BigInt::operator>>=(uint64_t shift)
{
    const uint64_t removed_items_count = shift / (sizeof(bigint_base_t) * 8);
    const uint64_t real_shift = shift % (sizeof(bigint_base_t) * 8);

    if (removed_items_count >= data.size())
    {
        data.resize(0);
        normalize();
        return *this;
    }

    data.resize(data.size() - removed_items_count);
    bigint_base_t shifted_val = 0;

    std::transform(
        data.crbegin(), data.crend(), data.rbegin(),
        [real_shift, &shifted_val](const bigint_base_t &v)
        {
            const bigint_base_t transformed = (v >> real_shift) | shifted_val;
            shifted_val = static_cast<bigint_base_t>(
                (static_cast<double_width_t<bigint_base_t>>(v) << (sizeof(bigint_base_t) * 8)) >> real_shift);
            return transformed;
        });

    normalize();
    return *this;
}

}  // namespace yabil::bigint
