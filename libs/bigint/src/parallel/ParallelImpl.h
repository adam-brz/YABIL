#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstdint>
#include <span>
#include <vector>

namespace yabil::bigint::parallel
{

std::size_t get_thread_count();
void set_thread_count(std::size_t thread_count);

std::vector<bigint_base_t> parallel_add_unsigned(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);
std::vector<bigint_base_t> parallel_karatsuba(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);

}  // namespace yabil::bigint::parallel
