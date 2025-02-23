#pragma once

#include <yabil/bigint/BigInt.h>

#include <span>
#include <vector>

namespace yabil::parallel
{

std::size_t get_thread_count();
void set_thread_count(std::size_t thread_count);

std::vector<bigint::bigint_base_t> parallel_add_unsigned(const std::span<const bigint::bigint_base_t> &a,
                                                         const std::span<const bigint::bigint_base_t> &b);

std::vector<bigint::bigint_base_t> parallel_karatsuba(const std::span<const bigint::bigint_base_t> &a,
                                                      const std::span<const bigint::bigint_base_t> &b);

}  // namespace yabil::parallel
