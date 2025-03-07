#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/arithmetic/Add.h>
#include <yabil/bigint/arithmetic/Mul.h>
#include <yabil/utils/IterUtils.h>
#include <yabil/utils/TypeUtils.h>

#include <span>
#include <vector>

#include "impl/Arithmetic.h"

namespace yabil::bigint::arithmetic
{

std::vector<bigint_base_t> mul_unsigned_basecase(const std::span<const bigint_base_t> &a,
                                                 const std::span<const bigint_base_t> &b)
{
    std::vector<bigint_base_t> result(a.size() + b.size(), 0);
    const auto [longer, shorter] = impl::get_longer_shorter(&a, &b);

    for (std::size_t i = 0; i < shorter->size(); ++i)
    {
        utils::double_width_t<bigint_base_t> carry = 0;
        std::size_t j;
        for (j = 0; j < longer->size(); ++j)
        {
            carry += result[i + j] + utils::safe_mul((*longer)[j], (*shorter)[i]);
            result[i + j] = static_cast<bigint_base_t>(carry);
            carry >>= BigInt::digit_size_bits;
        }
        if (carry)
        {
            result[i + longer->size()] += static_cast<bigint_base_t>(carry);
        }
    }

    return result;
}

std::vector<bigint_base_t> mul_unsigned_karatsuba(const std::span<const bigint_base_t> &a,
                                                  const std::span<const bigint_base_t> &b)
{
    const auto &config = BigIntGlobalConfig::instance();

    if (a.size() < config.karatsuba_threshold || b.size() < config.karatsuba_threshold)
    {
        return mul_unsigned_basecase(a, b);
    }

    const int m2 = static_cast<int>(std::max(a.size(), b.size()) / 2);

    const std::span<const bigint_base_t> low1 = utils::make_span(a.begin(), utils::safe_advance(a.begin(), m2, a));
    const std::span<const bigint_base_t> high1 = utils::make_span(utils::safe_advance(a.begin(), m2, a), a.end());

    const std::span<const bigint_base_t> low2 = utils::make_span(b.begin(), utils::safe_advance(b.begin(), m2, b));
    const std::span<const bigint_base_t> high2 = utils::make_span(utils::safe_advance(b.begin(), m2, b), b.end());

    const auto lh1 = add_unsigned(low1, high1);
    const auto lh2 = add_unsigned(low2, high2);

    const auto z0 = BigInt(mul_unsigned_karatsuba(low1, low2));
    const auto z1 = BigInt(mul_unsigned_karatsuba(lh1, lh2));
    const auto z2 = BigInt(mul_unsigned_karatsuba(high1, high2));

    constexpr auto digit_bit_size = std::numeric_limits<bigint_base_t>::digits;
    const uint64_t shift_val = static_cast<uint64_t>(m2) * digit_bit_size;
    auto result = (z2 << (shift_val * 2UL)) + ((z1 - z2 - z0) << shift_val) + z0;
    return result.raw_data();
}

}  // namespace yabil::bigint::arithmetic
