#include "StrConversion.h"

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/io.h>
#include <yabil/utils/IterUtils.h>

#include <cassert>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "StringConversionUtils.h"
#include "impl/Arithmetic.h"

namespace yabil::bigint::impl
{
namespace
{

static constexpr uint64_t conversion_base_10_to_18 = 1'000'000'000'000'000'000ULL;
static constexpr uint64_t conversion_base_10_to_9 = 1'000'000'000ULL;
static constexpr uint64_t conversion_base_10_to_4 = 10'000ULL;

static constexpr auto conversion_base =
    std::is_same_v<bigint_base_t, std::uint64_t> ? conversion_base_10_to_18 :      //
        (std::is_same_v<bigint_base_t, std::uint32_t> ? conversion_base_10_to_9 :  //
             conversion_base_10_to_4);

static constexpr auto conversion_decimal_places_count = std::is_same_v<bigint_base_t, std::uint64_t> ? 18 :      //
                                                            (std::is_same_v<bigint_base_t, std::uint32_t> ? 9 :  //
                                                                 4);

static constexpr int base_8_digits = 21;  // each octal digit is 3 bits, so 63 bits is 21 digits
static constexpr int base_8_digit_bits = 3;
static constexpr int base_8_shift_for_64_bits = base_8_digits * base_8_digit_bits;

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

std::pair<std::string_view, Sign> extract_sign(const std::string_view& str)
{
    assert(str.size() > 0);

    const char first = str.front();
    const bool hasSign = (first == '-') || (first == '+');

    if (hasSign && str.size() == 1)
    {
        throw std::invalid_argument("Invalid number format");
    }

    const Sign sign = hasSign ? (first == '-' ? Sign::Minus : Sign::Plus) : Sign::Plus;
    return std::make_pair(hasSign ? str.substr(1) : str, sign);
}

std::string to_string_any(const BigInt& number, const unsigned base)
{
    std::string str_number;
    BigInt n = number.abs();

    do
    {
        auto [quotient, remainder] = n.divide(BigInt{base});
        n = std::move(quotient);
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
            *digit_it = current / conversion_base;
            carry = current % conversion_base;
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
        result += std::to_string(*result_digit_it + conversion_base).substr(1);
    }

    return result;
}

BigInt from_string_any(const std::string_view& str, const unsigned base)
{
    assert(str.size() > 0);

    const char first = str.front();
    const bool hasSign = (first == '-') || (first == '+');

    std::vector<bigint_base_t> data;

    if (!hasSign)
    {
        const int converted = get_digit_value(std::tolower(first));
        check_conversion(first, static_cast<unsigned>(converted), base);
        data.push_back(converted);
    }

    BigInt result{std::move(data)};

    for (auto it = str.cbegin() + 1; it != str.cend(); ++it)
    {
        const int converted = get_digit_value(std::tolower(*it));
        check_conversion(*it, static_cast<unsigned>(converted), base);
        result *= BigInt{base};
        result += BigInt{converted};
    }

    result.set_sign(hasSign ? (first == '-' ? Sign::Minus : Sign::Plus) : Sign::Plus);
    return result;
}

BigInt from_string_2(const std::string_view& str)
{
    const auto [number_str, sign] = extract_sign(str);

    std::vector<bigint_base_t> data;
    data.reserve(number_str.size() / BigInt::digit_size_bits + 1);

    std::size_t processed = 0;
    int window_end = static_cast<int>(number_str.size());

    while (window_end > 0)
    {
        const int window_start = std::max(0, window_end - BigInt::digit_size_bits);
        const int window_len = window_end - window_start;

        const auto number_chunk = number_str.substr(window_start, window_len);
        const auto chunk_value = std::stoull(std::string{number_chunk}, &processed, 2);

        if (static_cast<int>(processed) != window_len)
        {
            throw std::invalid_argument("Invalid number format");
        }

        data.push_back(chunk_value);
        window_end = window_start;
    }

    BigInt result{std::move(data)};
    result.set_sign(sign);
    return result;
}

BigInt from_string_8(const std::string_view& str)
{
    const auto [number_str, sign] = extract_sign(str);

    BigInt result;

    std::size_t processed = 0;
    int window_end = static_cast<int>(number_str.size());
    uint64_t shift = 0;

    while (window_end > 0)
    {
        const int window_start = std::max(0, window_end - base_8_digits);
        const int window_len = window_end - window_start;

        const auto number_chunk = number_str.substr(window_start, window_len);
        const auto chunk_value = std::stoull(std::string{number_chunk}, &processed, 8);

        if (static_cast<int>(processed) != window_len)
        {
            throw std::invalid_argument("Invalid number format");
        }

        result += BigInt{chunk_value} << shift;
        shift += base_8_shift_for_64_bits;

        window_end = window_start;
    }

    result.set_sign(sign);
    return result;
}

BigInt from_string_10(const std::string_view& str)
{
    const auto [number_str, sign] = extract_sign(str);

    BigInt result;
    BigInt decimal_place{1};

    std::size_t processed = 0;
    int window_end = static_cast<int>(number_str.size());

    while (window_end > 0)
    {
        const int window_start = std::max(0, window_end - conversion_decimal_places_count);
        const int window_len = window_end - window_start;

        const auto number_chunk = number_str.substr(window_start, window_len);
        const auto chunk_value = std::stoull(std::string{number_chunk}, &processed);

        if (static_cast<int>(processed) != window_len)
        {
            throw std::invalid_argument("Invalid number format");
        }

        result += BigInt{chunk_value} * decimal_place;
        decimal_place *= BigInt{conversion_base};

        window_end = window_start;
    }

    result.set_sign(sign);
    return result;
}

BigInt from_string_16(const std::string_view& str)
{
    const auto [number_str, sign] = extract_sign(str);

    static constexpr int hex_digits_in_digit = BigInt::digit_size_bits / 4;

    std::vector<bigint_base_t> data;
    data.reserve(number_str.size() / hex_digits_in_digit + 1);

    std::size_t processed = 0;
    int window_end = static_cast<int>(number_str.size());

    while (window_end > 0)
    {
        const int window_start = std::max(0, window_end - hex_digits_in_digit);
        const int window_len = window_end - window_start;

        const auto number_chunk = number_str.substr(window_start, window_len);
        const auto chunk_value = std::stoull(std::string{number_chunk}, &processed, 16);

        if (static_cast<int>(processed) != window_len)
        {
            throw std::invalid_argument("Invalid number format");
        }

        data.push_back(chunk_value);
        window_end = window_start;
    }

    BigInt result{std::move(data)};
    result.set_sign(sign);
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
        case 10:
            return to_string_10(number);
        case 16:
            return to_string_16(number);
        default:
            return to_string_any(number, base);
    }
}

BigInt from_string(const std::string_view& str, const unsigned base)
{
    if (str.size() == 0)
    {
        return BigInt{};
    }

    switch (base)
    {
        case 2:
            return from_string_2(str);
        case 8:
            return from_string_8(str);
        case 10:
            return from_string_10(str);
        case 16:
            return from_string_16(str);
        default:
            return from_string_any(str, base);
    }
}

}  // namespace yabil::bigint::impl
