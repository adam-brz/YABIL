#pragma once

#include <yabil/bigint/BigInt.h>

#include <span>
#include <vector>

namespace yabil::bigint::parallel
{

std::vector<bigint_base_t> parallel_add_unsigned(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);
std::vector<bigint_base_t> parallel_karatsuba(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);

}  // namespace yabil::bigint::parallel
