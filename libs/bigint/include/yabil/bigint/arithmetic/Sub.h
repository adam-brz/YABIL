#pragma once

#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/bigint_export.h>

#include <span>
#include <vector>

namespace yabil::bigint::arithmetic
{

YABIL_BIGINT_EXPORT std::vector<bigint_base_t> sub_unsigned(const std::span<const bigint_base_t> &a,
                                                            const std::span<const bigint_base_t> &b);

YABIL_BIGINT_EXPORT std::vector<bigint_base_t> &decrement_unsigned(std::vector<bigint_base_t> &n);

}  // namespace yabil::bigint::arithmetic
