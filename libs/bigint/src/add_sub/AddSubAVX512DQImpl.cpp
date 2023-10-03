#include <immintrin.h>

#include <cassert>
#include <limits>

#include "AddSub.h"
#include "Arithmetic.h"

namespace yabil::bigint
{

__m512i avx_add512(__m512i A, __m512i B, uint32_t *carry)
{
    static const __m512i MAX_WORD = _mm512_set1_epi64(static_cast<int64_t>(0xffffffffffffffff));
    const __m512i s = _mm512_add_epi64(A, B);
    __mmask16 c = _mm512_cmplt_epu64_mask(s, A);
    __mmask16 m = _mm512_cmpeq_epi64_mask(s, MAX_WORD);

    c <<= 1;
    c = _mm512_kor(c, *carry);
    c += m;
    *carry = c >> 8;
    m = _mm512_kxor(m, c);

    return _mm512_mask_sub_epi64(s, m, s, MAX_WORD);
}

__m512i avx_sub512(__m512i A, __m512i B, uint32_t *carry)
{
    static const __m512i MAX_WORD = _mm512_set1_epi64(static_cast<int64_t>(0xffffffffffffffff));
    const __m512i s = _mm512_sub_epi64(A, B);
    __mmask16 c = _mm512_cmpgt_epu64_mask(s, A);
    __mmask16 m = _mm512_cmpeq_epi64_mask(s, _mm512_setzero_si512());

    c <<= 1;
    c = _mm512_kor(c, *carry);
    c += m;
    *carry = c >> 8;
    m = _mm512_kxor(m, c);

    return _mm512_mask_add_epi64(s, m, s, MAX_WORD);
}

void add_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r)
{
    const auto a_size_bytes = a_size * sizeof(bigint_base_t);
    const auto b_size_bytes = b_size * sizeof(bigint_base_t);

    assert(a_size_bytes >= b_size_bytes);

    constexpr auto avx512_data_size_bytes = 512 / 8;
    const auto max_avx_iters = b_size_bytes / avx512_data_size_bytes;
    uint32_t carry = 0;

    for (unsigned i = 0; i < max_avx_iters; ++i)
    {
        const __m512i A_v = _mm512_loadu_si512(&(reinterpret_cast<const __m512i_u *>(a)[i]));
        const __m512i B_v = _mm512_loadu_si512(&(reinterpret_cast<const __m512i_u *>(b)[i]));
        const __m512i R_v = avx_add512(A_v, B_v, &carry);
        _mm512_storeu_si512(&(reinterpret_cast<__m512i_u *>(r)[i]), R_v);
    }

    a += max_avx_iters * avx512_data_size_bytes / sizeof(*a);
    b += max_avx_iters * avx512_data_size_bytes / sizeof(*b);
    r += max_avx_iters * avx512_data_size_bytes / sizeof(*r);

    const auto a_unaligned = (a_size_bytes - max_avx_iters * avx512_data_size_bytes) / sizeof(*a);
    const auto b_unaligned = (b_size_bytes - max_avx_iters * avx512_data_size_bytes) / sizeof(*b);

    add_plain_arrays(a, a_unaligned, b, b_unaligned, r, carry);
}

// Requires a_size > b_size
void sub_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r)
{
    const auto a_size_bytes = a_size * sizeof(bigint_base_t);
    const auto b_size_bytes = b_size * sizeof(bigint_base_t);

    assert(a_size_bytes >= b_size_bytes);

    constexpr auto avx512_data_size_bytes = 512 / 8;
    const auto max_avx_iters = b_size_bytes / avx512_data_size_bytes;
    uint32_t borrow = 0;

    for (unsigned i = 0; i < max_avx_iters; ++i)
    {
        const __m512i A_v = _mm512_loadu_si512(&(reinterpret_cast<const __m512i_u *>(a)[i]));
        const __m512i B_v = _mm512_loadu_si512(&(reinterpret_cast<const __m512i_u *>(b)[i]));
        const __m512i R_v = avx_sub512(A_v, B_v, &borrow);
        _mm512_storeu_si512(&(reinterpret_cast<__m512i_u *>(r)[i]), R_v);
    }

    a += max_avx_iters * avx512_data_size_bytes / sizeof(*a);
    b += max_avx_iters * avx512_data_size_bytes / sizeof(*b);
    r += max_avx_iters * avx512_data_size_bytes / sizeof(*r);

    const auto a_unaligned = (a_size_bytes - max_avx_iters * avx512_data_size_bytes) / sizeof(*a);
    const auto b_unaligned = (b_size_bytes - max_avx_iters * avx512_data_size_bytes) / sizeof(*b);

    sub_plain_arrays(a, a_unaligned, b, b_unaligned, r, borrow);
}

}  // namespace yabil::bigint
