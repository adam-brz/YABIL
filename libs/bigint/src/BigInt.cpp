#include <yabil/bigint/BigInt.h>

#include <SafeInt.hpp>
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
    for (std::size_t i = 0; (i < sizeof(int64_t) / sizeof(bigint_base_t)) && (i < data.size()); ++i)
    {
        result |= static_cast<int64_t>(data[i]) << i * sizeof(bigint_base_t) * 8;
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
    // BigInt result(*this);
    // // if (sign == other.sign)
    // // {
    // //     // simple add, sign = sign
    // // }
    // // else
    // // {
    // //     if (sign == Sign::Minus)
    // //     {
    // //         // simple other - this
    // //     }
    // //     else
    // //     {
    // //         // simple this - other
    // //     }
    // // }
    // result += other;
    // return result;
    std::vector<bigint_base_t> result_data;
    bool carry = false;
    for (std::size_t i = 0; i < std::max(data.size(), other.data.size()); ++i)
    {
        const bigint_base_t self_value = value_at(i);
        const bigint_base_t other_value = other.value_at(i);

        bigint_base_t self_with_carry = self_value;
        if (carry && !SafeAdd(self_value, 1, self_with_carry))
        {
            result_data.push_back(other_value);
            continue;
        }

        bigint_base_t dummy;
        carry = !SafeAdd(self_with_carry, other_value, dummy);
        result_data.push_back(self_with_carry + other_value);
    }
    if (carry)
    {
        result_data.push_back(1);
    }
    return BigInt(std::move(result_data));
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
