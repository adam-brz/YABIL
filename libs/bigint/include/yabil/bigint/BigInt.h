#pragma once

#include <cstdint>
#include <string>
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
    explicit BigInt(uint64_t number = 0, Sign sign = Sign::Plus);
    explicit BigInt(int64_t number);

    int64_t to_int() const;
    bool is_negative() const;
    const std::vector<bigint_base_t> &raw_data() const;

    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;
    BigInt operator+(const BigInt &other) const;
    BigInt &operator+=(const BigInt &other);
};

}  // namespace yabil::bigint
