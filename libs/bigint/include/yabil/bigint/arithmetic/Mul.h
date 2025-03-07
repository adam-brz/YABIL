#pragma once

#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/bigint_export.h>

#include <span>
#include <vector>

namespace yabil::bigint::arithmetic
{

YABIL_BIGINT_EXPORT std::vector<bigint_base_t> mul_unsigned_basecase(const std::span<const bigint_base_t> &a,
                                                                     const std::span<const bigint_base_t> &b);

YABIL_BIGINT_EXPORT std::vector<bigint_base_t> mul_unsigned_karatsuba(const std::span<const bigint_base_t> &a,
                                                                      const std::span<const bigint_base_t> &b);

}  // namespace yabil::bigint::arithmetic
