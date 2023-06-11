#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstdint>

#ifdef __AVX2__

void avx2_add(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
              std::size_t b_size, yabil::bigint::bigint_base_t *r);

void avx2_sub(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
              std::size_t b_size, yabil::bigint::bigint_base_t *r);

#endif
