#include "StrConversion.h"

#include <cassert>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>

#include "StringConversionUtils.h"
#include "impl/Arithmetic.h"

namespace yabil::bigint::impl
{
namespace
{

std::string merge_converted_parts(const std::vector<std::string>& parts, const int part_size_digits, const Sign sign)
{
    std::string result;
    result.reserve(parts.size() * part_size_digits + 1);

    if (sign == Sign::Minus)
    {
        result += '-';
    }

    for (auto it = parts.rbegin(); it != parts.rend(); ++it)
    {
        result.insert(result.end(), it->begin(), it->end());
    }

    return result;
}

std::string to_string_any(const BigInt& number, const unsigned base)
{
    std::string str_number;
    BigInt n = number.abs();

    do
    {
        const auto [quotient, remainder] = n.divide(BigInt{base});
        n = quotient;
        str_number.append(1, get_digit_char(static_cast<int>(remainder.to<int>())));
    } while (!n.is_zero());

    if (number.get_sign() == Sign::Minus)
    {
        str_number += '-';
    }

    std::reverse(str_number.begin(), str_number.end());
    return str_number;
}

std::string to_string_2(const BigInt& number)
{
    const auto& data = number.raw_data();

    std::vector<std::string> result_parts;
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        const auto& digit = data[i];

        std::stringstream ss;
        ss << std::bitset<BigInt::digit_size_bits>(digit);
        result_parts.push_back(ss.str());
    }

    assert(!result_parts.empty());
    result_parts.back().erase(0, result_parts.back().find_first_not_of('0'));

    return merge_converted_parts(result_parts, BigInt::digit_size_bits, number.get_sign());
}

std::string to_string_8(const BigInt& number)
{
    static constexpr int base_8_digits = 21;  // each octal digit is 3 bits, so 63 bits is 21 digits
    static constexpr int base_8_digit_bits = 3;
    static constexpr int base_8_shift_for_64_bits = base_8_digits * base_8_digit_bits;

    BigInt n = number.abs();
    std::vector<std::string> result_parts;

    do
    {
        const uint64_t remainder_value = n.to<uint64_t>() & ~(1ULL << base_8_shift_for_64_bits);  // first 63 bits
        n >>= 63;

        std::stringstream ss;
        ss << std::oct << std::setfill('0') << std::setw(n.is_zero() ? 0 : base_8_digits) << remainder_value;
        result_parts.push_back(ss.str());

    } while (!n.is_zero());

    return merge_converted_parts(result_parts, base_8_digits, number.get_sign());
}

std::string to_string_16(const BigInt& number)
{
    static constexpr int amount_of_bits_in_hex_digit = 4;
    const auto& data = number.raw_data();

    std::vector<std::string> result_parts;
    for (const auto& digit : data)
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(BigInt::digit_size_bits / amount_of_bits_in_hex_digit)
           << digit;
        result_parts.push_back(ss.str());
    }

    assert(!result_parts.empty());
    result_parts.back().erase(0, result_parts.back().find_first_not_of('0'));

    return merge_converted_parts(result_parts, BigInt::digit_size_bits / amount_of_bits_in_hex_digit,
                                 number.get_sign());
}

std::string to_string_10(const BigInt& number)
{
    constexpr uint64_t base_10_to_18 = 1'000'000'000'000'000'000ULL;
    constexpr uint64_t base_10_to_9 = 1'000'000'000ULL;
    constexpr uint64_t base_10_to_4 = 10'000ULL;

    constexpr auto base = std::is_same_v<bigint_base_t, std::uint64_t> ? base_10_to_18 :      //
                              (std::is_same_v<bigint_base_t, std::uint32_t> ? base_10_to_9 :  //
                                   base_10_to_4);

    static_assert(sizeof(bigint_base_t) >= 2, "This function cannot be used with types smaller than 16 bits");

    auto data = number.raw_data();
    std::vector<bigint_base_t> result_parts;

    const auto is_zero = [](const auto& data) { return data.empty() || (data.size() == 1 && data[0] == 0); };

    while (!is_zero(data))
    {
        bigint_base_t carry = 0;

        for (auto digit_it = data.rbegin(); digit_it != data.rend(); ++digit_it)
        {
            const auto current =
                static_cast<utils::double_width_t<bigint_base_t>>(carry) << BigInt::digit_size_bits | *digit_it;
            *digit_it = current / base;
            carry = current % base;
        }

        result_parts.push_back(carry);
        impl::remove_trailing_zeros(data);
    }

    // Convert chunks to string
    std::string result;

    if (number.get_sign() == Sign::Minus)
    {
        result += '-';
    }

    result += std::to_string(result_parts.back());

    for (auto result_digit_it = result_parts.rbegin() + 1; result_digit_it != result_parts.rend(); ++result_digit_it)
    {
        result += std::to_string(*result_digit_it + base).substr(1);
    }

    return result;
}

}  // namespace

std::string to_string(const BigInt& number, const unsigned base)
{
    if (number.is_zero())
    {
        return "0";
    }

    switch (base)
    {
        case 2:
            return to_string_2(number);
        case 8:
            return to_string_8(number);
        case 16:
            return to_string_16(number);
        case 10:
            return to_string_10(number);
        default:
            return to_string_any(number, base);
    }
}

}  // namespace yabil::bigint::impl
