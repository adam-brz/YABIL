#include <yabil/bigint/Parallel.h>
#include <yabil/math/Math.h>

#include <bit>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <tuple>

namespace yabil::math
{

namespace
{

yabil::bigint::BigInt pow_recursive(const yabil::bigint::BigInt &number, const yabil::bigint::BigInt &n)
{
    yabil::bigint::BigInt one_const(1);
    if (n.is_zero()) return one_const;
    if (n == one_const) return number;
    if (n.is_even()) return pow_recursive(number * number, n >> 1);
    return pow_recursive(number * number, n >> 1) * number;
}

}  // namespace

yabil::bigint::BigInt pow(const yabil::bigint::BigInt &number, const yabil::bigint::BigInt &n)
{
    const auto new_sign = (number.get_sign() == yabil::bigint::Sign::Minus && !n.is_even()) ? yabil::bigint::Sign::Minus
                                                                                            : yabil::bigint::Sign::Plus;
    yabil::bigint::BigInt result = pow_recursive(number, n);
    result.set_sign(new_sign);
    return result;
}

yabil::bigint::BigInt pow(const yabil::bigint::BigInt &number, const yabil::bigint::BigInt &n,
                          const yabil::bigint::BigInt &mod)
{
    if (number.is_negative() || n.is_negative() || mod.is_negative())
    {
        throw std::invalid_argument("Cannot calculate power in modular arithmetic for negative number");
    }

    if (n.is_zero())
    {
        return yabil::bigint::BigInt(1);
    }

    yabil::bigint::BigInt result(1);
    yabil::bigint::BigInt base(number % mod);
    yabil::bigint::BigInt exponent(n);

    if (base.is_zero())
    {
        return yabil::bigint::BigInt();
    }

    while (!exponent.is_zero())
    {
        if (!exponent.is_even())
        {
            result = bigint::parallel::multiply(result, base) % mod;
        }
        exponent >>= 1;
        base = bigint::parallel::multiply(base, base) % mod;
    }
    return result;
}

yabil::bigint::BigInt factorial(uint64_t n)
{
    yabil::bigint::BigInt result(1U);
    for (uint64_t i = 1; i <= n; ++i)
    {
        result = bigint::parallel::multiply(result, yabil::bigint::BigInt(i));
    }
    return result;
}

uint64_t log2_int(const yabil::bigint::BigInt &number)
{
    if (number.is_negative() || number.is_zero())
    {
        throw std::invalid_argument("Logarithm argument must be greater than 0");
    }

    constexpr auto item_size_bits = sizeof(number.raw_data().front()) * 8;
    const auto bit_size = (number.raw_data().size() - 1) * item_size_bits;
    const auto last_item = number.raw_data().back();
    const auto last_one_pos = item_size_bits - std::countl_zero(last_item);
    return bit_size + last_one_pos - 1;
}

double log2(const yabil::bigint::BigInt &number)
{
    constexpr auto item_size_bits = sizeof(yabil::bigint::bigint_base_t) * 8;
    constexpr int result_iter_count = 64 / item_size_bits;

    const auto bit_shift_quotient_removal = std::countl_zero(number.raw_data().back()) + 1;
    uint64_t raw_fraction = 0;

    int i;
    for (i = 0; i < result_iter_count && i < static_cast<int>(number.raw_data().size()); ++i)
    {
        const uint64_t fraction_part = static_cast<uint64_t>(number.raw_data()[number.raw_data().size() - 1 - i])
                                       << bit_shift_quotient_removal;
        raw_fraction |= fraction_part << (64 - (i + 1) * item_size_bits);
    }

    if (i < static_cast<int>(number.raw_data().size()))
    {
        raw_fraction |= static_cast<uint64_t>(number.raw_data()[number.raw_data().size() - 1 - i]) >>
                        (item_size_bits - bit_shift_quotient_removal);
    }

    raw_fraction = (raw_fraction >> 12) | 0x3ff0000000000000;
    const double remaining_part = *static_cast<double *>(reinterpret_cast<void *>(&raw_fraction));
    return static_cast<double>(log2_int(number)) + std::log2(remaining_part);
}

double log(const yabil::bigint::BigInt &number, const yabil::bigint::BigInt &base)
{
    return log2(number) / log2(base);
}

double log(const yabil::bigint::BigInt &number, double base)
{
    return log2(number) / std::log2(base);
}

yabil::bigint::BigInt gcd(yabil::bigint::BigInt number, yabil::bigint::BigInt other)
{
    if (number.is_zero()) return other;
    if (other.is_zero()) return number;

    uint64_t power_of_two_divisor_number = 0;
    uint64_t power_of_two_divisor_other = 0;

    while (!number.get_bit(power_of_two_divisor_number))
    {
        power_of_two_divisor_number += 1;
    }

    while (!other.get_bit(power_of_two_divisor_other))
    {
        power_of_two_divisor_other += 1;
    }

    uint64_t common_power_of_2 = std::min(power_of_two_divisor_number, power_of_two_divisor_other);
    number >>= common_power_of_2;
    other >>= common_power_of_2;

    if (number.is_int64() && other.is_int64())
    {
        return yabil::bigint::BigInt(std::gcd(number.to_int(), other.to_int())) << common_power_of_2;
    }

    while (!other.is_zero())
    {
        std::tie(number, other) = std::make_pair(other, number % other);
    }

    return number << common_power_of_2;
}

std::pair<yabil::bigint::BigInt, std::pair<yabil::bigint::BigInt, yabil::bigint::BigInt>> extended_gcd(
    const yabil::bigint::BigInt &a, const yabil::bigint::BigInt &b)
{
    using BezoutCoefficientsType = std::pair<yabil::bigint::BigInt, yabil::bigint::BigInt>;
    yabil::bigint::BigInt old_r{a}, r{b}, old_s{1}, s{0}, old_t{0}, t{1};

    while (!r.is_zero())
    {
        const auto [quotient, _] = bigint::parallel::divide(old_r, r);
        std::tie(old_r, r) = BezoutCoefficientsType{r, old_r - bigint::parallel::multiply(quotient, r)};
        std::tie(old_s, s) = BezoutCoefficientsType{s, old_s - bigint::parallel::multiply(quotient, s)};
        std::tie(old_t, t) = BezoutCoefficientsType{t, old_t - bigint::parallel::multiply(quotient, t)};
    }

    return {old_r, {old_s, old_t}};
}

yabil::bigint::BigInt mod_inverse(const yabil::bigint::BigInt &a, const yabil::bigint::BigInt &n)
{
    const auto result = extended_gcd(a, n);
    if (result.first > yabil::bigint::BigInt(1))
    {
        throw std::runtime_error("number: " + a.to_str() + " is not invertible");
    }
    const auto &x = result.second.first;
    return x.is_negative() ? x + n : x;
}

yabil::bigint::BigInt sqrt(const yabil::bigint::BigInt &n)
{
    yabil::bigint::BigInt tmp = n;
    yabil::bigint::BigInt result;

    if (n.is_zero())
    {
        return bigint::BigInt();
    }

    if (n.is_negative())
    {
        throw std::invalid_argument("Cannot calculate square root for negative integer");
    }

    do
    {
        result = tmp;
        tmp = (result + n / result) >> 1;
    } while (tmp < result);
    return result;
}

yabil::bigint::BigInt root(const yabil::bigint::BigInt &n, const yabil::bigint::BigInt &k)
{
    if (n.is_zero())
    {
        return bigint::BigInt();
    }

    if (n.is_negative())
    {
        throw std::invalid_argument("Cannot calculate square root for negative integer");
    }

    bigint::BigInt tmp = n;
    bigint::BigInt result;
    const bigint::BigInt one(1);

    do
    {
        result = tmp;
        tmp = ((k - one) * result + n / pow(result, k - one)) / k;
    } while (tmp < result);
    return result;
}

}  // namespace yabil::math
