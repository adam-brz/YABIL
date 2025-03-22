#pragma once

#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/bigint_export.h>

#include <cstdlib>

namespace yabil::bigint::arithmetic
{

/// @brief Low level array addition with carry propagation.
/// @details s_size has to be greater or equal b_size, otherwise behavior is undefined.
/// @param a Pointer to first item of number a.
/// @param a_size Size in elements of array with start at a.
/// @param b Pointer to first item of number b.
/// @param b_size Size in elements of array with start at b.
/// @param r output pointer for array to store sum. Output array must have size greater or equal a_size + 1.
/// @param carry Additional carry to begin addition with.
YABIL_BIGINT_EXPORT void add_arrays_with_carry(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b,
                                               std::size_t b_size, bigint_base_t *r, bigint_base_t carry = 0);

/// @brief Low level array subtraction with borrow propagation.
/// @details s_size has to be greater or equal b_size, otherwise behavior is undefined.
/// @param a Pointer to first item of number a.
/// @param a_size Size in elements of array with start at a.
/// @param b Pointer to first item of number b.
/// @param b_size Size in elements of array with start at b.
/// @param r output pointer for array to store difference. Output array must have size greater or equal a_size.
/// @param borrow Additional borrow to begin subtraction with.
YABIL_BIGINT_EXPORT void sub_arrays_with_borrow(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b,
                                                std::size_t b_size, bigint_base_t *r, bigint_base_t borrow = 0);

}  // namespace yabil::bigint::arithmetic
