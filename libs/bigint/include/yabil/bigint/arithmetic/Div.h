#pragma once

#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/bigint_export.h>

#include <span>
#include <utility>
#include <vector>

namespace yabil::bigint::arithmetic
{

YABIL_BIGINT_EXPORT std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned(
    const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b);

YABIL_BIGINT_EXPORT std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned_basecase(
    const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b);

YABIL_BIGINT_EXPORT std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned_unbalanced(
    const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b);

YABIL_BIGINT_EXPORT std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned_recursive(
    const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b);

}  // namespace yabil::bigint::arithmetic
