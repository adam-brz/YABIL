#include <SafeOperators.h>
#include <yabil/bigint/BigInt.h>

#include <algorithm>
#include <iostream>
#include <ranges>

namespace yabil::bigint
{

BigInt::BigInt(const std::vector<bigint_base_t> &raw_data, Sign sign) : data(raw_data), sign(sign) {}

int64_t BigInt::to_int() const
{
    if (data.size() == 0)
    {
        return 0;
    }

    int64_t result = 0;
    for (std::size_t i = 0; (i < data.size()) && (i < sizeof(int64_t) / sizeof(bigint_base_t)); ++i)
    {
        result |= static_cast<int64_t>(data[i]) << (i * sizeof(bigint_base_t) * 8);
    }

    return is_negative() ? -result : result;
}

bool BigInt::is_negative() const
{
    return sign == Sign::Minus;
}

bigint_base_t BigInt::value_at(std::size_t index) const
{
    return index < data.size() ? data.at(index) : 0;
}

const std::vector<bigint_base_t> &BigInt::raw_data() const
{
    return data;
}

Sign BigInt::get_sign() const
{
    return sign;
}

bool BigInt::operator==(const BigInt &other) const
{
    return (data.size() == 0 && other.data.size() == 0) || (data == other.data && sign == other.sign);
}

bool BigInt::operator!=(const BigInt &other) const
{
    return !(*this == other);
}

BigInt BigInt::operator+(const BigInt &other) const
{
    if (sign == other.sign)
    {
        return plain_add(other);
    }

    if (sign == Sign::Minus)
    {
        return other.plain_sub(*this);
    }

    return plain_sub(other);
}

BigInt BigInt::operator-(const BigInt &other) const
{
    if (sign != other.sign)
    {
        return plain_add(other);
    }

    if (sign == Sign::Plus)
    {
        return plain_sub(other);
    }

    return other.plain_sub(*this);
}

BigInt BigInt::plain_add(const BigInt &other) const
{
    std::vector<bigint_base_t> result_data;
    bigint_base_t carry = 0;

    const std::size_t result_size = std::max(data.size(), other.data.size());
    result_data.reserve(result_size + 1);

    for (std::size_t i = 0; i < result_size; ++i)
    {
        const bigint_base_t self_value = value_at(i);
        const bigint_base_t other_value = other.value_at(i);
        const auto addition_result = safe_add(self_value, other_value, carry);
        carry = static_cast<bigint_base_t>(addition_result >> (sizeof(carry) * 8));
        result_data.push_back(static_cast<bigint_base_t>(addition_result));
    }
    if (carry > 0)
    {
        result_data.push_back(carry);
    }
    return BigInt(std::move(result_data), sign);
}

BigInt BigInt::plain_sub(const BigInt &other) const
{
    return *this;
}

BigInt &BigInt::operator+=(const BigInt &other)
{
    // const int result_size = static_cast<int>(std::max(data.size(), other.data.size()));
    // for (int i = 0; i < result_size; ++i)
    // {
    //     const auto self_value = i < data.size() ? data[i] : 0;
    //     const auto other_value = i < other.data.size() ? other.data[i] : 0;
    //     bool carry = SafeAdd(self_value, other, data.at(i));

    // }
    return *this = *this + other;
}

}  // namespace yabil::bigint
