#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstdint>

#ifdef __AVX2__

void avx_add(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
             std::size_t b_size, yabil::bigint::bigint_base_t *r);

void avx_sub(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
             std::size_t b_size, yabil::bigint::bigint_base_t *r);

#endif
