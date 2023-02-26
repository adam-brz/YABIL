#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <vector>

namespace yabil::bigint
{

using bigint_base_t = uint64_t;

enum class Sign : uint8_t
{
    Plus,
    Minus
};

class BigInt
{
private:
    std::vector<bigint_base_t> data;
    Sign sign;

public:
    explicit BigInt(const std::vector<bigint_base_t> &raw_data, Sign sign = Sign::Plus);
    explicit BigInt(const std::string &str, int base = 10);
    explicit BigInt() : BigInt(bigint_base_t()) {}

    template <typename T, class = typename std::enable_if_t<std::is_signed_v<T>>>
    explicit BigInt(T number)
        : BigInt(static_cast<std::make_unsigned_t<T>>(std::abs(number)), number < 0 ? Sign::Minus : Sign::Plus)
    {
    }

    template <typename T, class = typename std::enable_if_t<std::is_unsigned_v<T>>>
    explicit BigInt(T number, Sign sign = Sign::Plus) : sign(sign)
    {
        constexpr int data_item_count = sizeof(T) / sizeof(bigint_base_t);
        if constexpr (data_item_count < 2)
        {
            data.push_back(static_cast<bigint_base_t>(number));
        }
        else
        {
            data.reserve(data_item_count);
            for (std::size_t i = 0; i < data_item_count; ++i)
            {
                data.push_back(static_cast<bigint_base_t>(number >> i * sizeof(bigint_base_t) * 8));
            }
        }
    }

    int64_t to_int() const;
    bool is_negative() const;
    bigint_base_t value_at(std::size_t index) const;

    const std::vector<bigint_base_t> &raw_data() const;

    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;
    BigInt operator+(const BigInt &other) const;
    BigInt &operator+=(const BigInt &other);
};

}  // namespace yabil::bigint
