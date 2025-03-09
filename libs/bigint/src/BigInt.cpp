#include <yabil/bigint/BigInt.h>

#include <algorithm>
#include <bit>
#include <cctype>
#include <cmath>
#include <functional>

#include "StringConversionUtils.h"
#include "impl/Arithmetic.h"
#include "impl/StrConversion.h"

namespace yabil::bigint
{

BigInt::BigInt(std::vector<bigint_base_t> &&raw_data, Sign sign) : data(std::move(raw_data)), sign(sign)
{
    normalize();
}

BigInt::BigInt(const std::span<const bigint_base_t> &raw_data, Sign sign) : sign(sign)
{
    const auto lastNonZeroDigitIt =
        std::find_if(raw_data.rbegin(), raw_data.rend(), [](const auto &v) { return v != 0; }).base();
    data.insert(data.begin(), raw_data.begin(), lastNonZeroDigitIt);
}

BigInt::BigInt(const std::string_view &str, unsigned base)
{
    *this = impl::from_string(str, base);
}

void BigInt::normalize()
{
    impl::remove_trailing_zeros(data);
    sign = is_zero() ? Sign::Plus : sign;
}

uint64_t BigInt::byte_size() const
{
    return data.size() * sizeof(bigint_base_t);
}

std::string BigInt::to_str(unsigned base) const
{
    return impl::to_string(*this, base);
}

bool BigInt::is_negative() const
{
    return sign == Sign::Minus;
}

bool BigInt::is_even() const
{
    return is_zero() || ((data.front() & 0x01) == 0);
}

bool BigInt::is_power_of_2() const
{
    uint64_t non_zero_bits = 0;
    for (const auto &digit : data)
    {
        non_zero_bits += std::popcount(digit);
        if (non_zero_bits > 1)
        {
            return false;
        }
    }
    return non_zero_bits == 1;
}

BigInt BigInt::abs() const
{
    return BigInt(data, Sign::Plus);
}

const std::vector<bigint_base_t> &BigInt::raw_data() const
{
    return data;
}

Sign BigInt::get_sign() const
{
    return sign;
}

void BigInt::set_sign(Sign new_sign)
{
    sign = new_sign;
}

bool BigInt::is_zero() const
{
    return data.size() == 0;
}

bool BigInt::get_bit(std::size_t n) const
{
    const auto item_index = n / BigInt::digit_size_bits;
    const auto bit_index = n % BigInt::digit_size_bits;

    if (item_index >= data.size())
    {
        return 0;
    }
    return (data[item_index] >> bit_index) & 0x01;
}

void BigInt::set_bit(std::size_t n, bool bit_value)
{
    const auto item_index = n / BigInt::digit_size_bits;
    const auto bit_index = n % BigInt::digit_size_bits;

    if (item_index >= data.size())
    {
        data.resize(item_index + 1, 0);
    }

    data[item_index] =
        (data[item_index] & ~(bigint_base_t(1) << bit_index)) | (static_cast<bigint_base_t>(bit_value) << bit_index);
    normalize();
}

}  // namespace yabil::bigint
