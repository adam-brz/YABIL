#include <yabil/bigint/BigInt.h>

#include <SafeInt.hpp>
#include <algorithm>
#include <iostream>
#include <ranges>

namespace yabil::bigint
{

BigInt::BigInt(const std::vector<bigint_base_t> &raw_data, Sign sign) : data(raw_data), sign(sign){};

BigInt::BigInt(uint64_t number, Sign sign) : sign(sign)
{
    for (int i = 0; i < sizeof(uint64_t) / sizeof(bigint_base_t); ++i)
    {
        data.push_back(static_cast<bigint_base_t>(number >> i * sizeof(bigint_base_t) * 8));
    }
}

BigInt::BigInt(int64_t number) : sign(number < 0 ? Sign::Minus : Sign::Plus)
{
    const int64_t abs_number = std::abs(number);
    for (int i = 0; i < sizeof(int64_t) / sizeof(bigint_base_t); ++i)
    {
        data.push_back(static_cast<bigint_base_t>(abs_number >> i * sizeof(bigint_base_t) * 8));
    }
}

BigInt::BigInt(const std::string &str, int base) {}

int64_t BigInt::to_int() const
{
    if (data.size() == 0)
    {
        return 0;
    }

    int64_t result = 0;
    for (int i = 0; (i < sizeof(int64_t) / sizeof(bigint_base_t)) && (i < data.size()); ++i)
    {
        result |= static_cast<int64_t>(data[i]) << i * sizeof(bigint_base_t) * 8;
    }

    return is_negative() ? -result : result;
}

bool BigInt::is_negative() const
{
    return sign == Sign::Minus;
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

    const int result_size = static_cast<int>(std::max(data.size(), other.data.size()));
    for (int i = 0; i < result_size; ++i)
    {
        const auto self_value = i < data.size() ? data[i] : 0;
        const auto other_value = i < other.data.size() ? other.data[i] : 0;

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
    return BigInt(result_data);
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
