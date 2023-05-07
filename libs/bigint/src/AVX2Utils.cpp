#include "AVX2Utils.h"

#ifdef __AVX2__

using namespace yabil::bigint;

#include <immintrin.h>

#include <limits>

#include "SafeOperators.h"

static const __m256i BROADCAST_MASK[16] = {
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000),
                      static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000),
                      static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000001)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000),
                      static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000000)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000),
                      static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000001)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000001),
                      static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000001),
                      static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000001)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000001),
                      static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000000)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000001),
                      static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000001)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000000),
                      static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000000),
                      static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000001)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000000),
                      static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000000)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000000),
                      static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000001)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000001),
                      static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000000)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000001),
                      static_cast<int64_t>(0x8000000000000000), static_cast<int64_t>(0x8000000000000001)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000001),
                      static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000000)),
    _mm256_set_epi64x(static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000001),
                      static_cast<int64_t>(0x8000000000000001), static_cast<int64_t>(0x8000000000000001)),
};

__m256i avx_add256(__m256i A, __m256i B, uint32_t *carry)
{
    A = _mm256_xor_si256(A, _mm256_set1_epi64x(static_cast<int64_t>(0x8000000000000000)));
    __m256i s = _mm256_add_epi64(A, B);
    __m256i cv = _mm256_cmpgt_epi64(A, s);
    __m256i mv = _mm256_cmpeq_epi64(s, _mm256_set1_epi64x(0x7fffffffffffffff));
    uint32_t c = _mm256_movemask_pd(_mm256_castsi256_pd(cv));
    uint32_t m = _mm256_movemask_pd(_mm256_castsi256_pd(mv));

    c = m + 2 * c;
    *carry += c;
    m ^= *carry;
    *carry >>= 4;
    m &= 0x0f;

    return _mm256_add_epi64(s, BROADCAST_MASK[m]);
}

__m256i avx_sub256(__m256i A, __m256i B, uint32_t *borrow)
{
    A = _mm256_xor_si256(A, _mm256_set1_epi64x(static_cast<int64_t>(0x8000000000000000)));
    __m256i s = _mm256_sub_epi64(A, B);
    __m256i cv = _mm256_cmpgt_epi64(s, A);
    __m256i mv = _mm256_cmpeq_epi64(s, _mm256_set1_epi64x(0x7fffffffffffffff));
    uint32_t c = _mm256_movemask_pd(_mm256_castsi256_pd(cv));
    uint32_t m = _mm256_movemask_pd(_mm256_castsi256_pd(mv));

    c = m + 2 * c;
    *borrow += c;
    m ^= *borrow;
    *borrow >>= 4;
    m &= 0x0f;

    return _mm256_sub_epi64(s, BROADCAST_MASK[m]);
}

// Requires a_size > b_size
void avx_add(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size, bigint_base_t *r)
{
    const auto max_avx_iters = b_size / 32;
    uint32_t carry = 0;

    for (unsigned i = 0; i < max_avx_iters; ++i)
    {
        const __m256i A_v = _mm256_loadu_si256(&(reinterpret_cast<const __m256i_u *>(a)[i]));
        const __m256i B_v = _mm256_loadu_si256(&(reinterpret_cast<const __m256i_u *>(b)[i]));
        const __m256i R_v = avx_add256(A_v, B_v, &carry);
        _mm256_storeu_si256(&(reinterpret_cast<__m256i_u *>(r)[i]), R_v);
    }

    a += max_avx_iters * 32 / sizeof(*a);
    b += max_avx_iters * 32 / sizeof(*b);
    r += max_avx_iters * 32 / sizeof(*r);

    const auto a_unaligned = (a_size - max_avx_iters * 32) / sizeof(*a);
    const auto b_unaligned = (b_size - max_avx_iters * 32) / sizeof(*b);

    std::size_t i;
    for (i = 0; i < b_unaligned; ++i)
    {
        const auto addition_result = safe_add(a[i], b[i], carry);
        carry = static_cast<uint32_t>(addition_result >> (sizeof(bigint_base_t) * 8));
        r[i] = static_cast<bigint_base_t>(addition_result);
    }

    for (; i < a_unaligned; ++i)
    {
        const auto addition_result = safe_add(a[i], carry);
        carry = static_cast<uint32_t>(addition_result >> (sizeof(bigint_base_t) * 8));
        r[i] = static_cast<bigint_base_t>(addition_result);
    }

    r[i] = carry;
}

// Requires a_size > b_size
void avx_sub(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size, bigint_base_t *r)
{
    const auto max_avx_iters = b_size / 32;
    uint32_t borrow = 0;

    for (unsigned i = 0; i < max_avx_iters; ++i)
    {
        const __m256i A_v = _mm256_loadu_si256(&(reinterpret_cast<const __m256i_u *>(a)[i]));
        const __m256i B_v = _mm256_loadu_si256(&(reinterpret_cast<const __m256i_u *>(b)[i]));
        const __m256i R_v = avx_sub256(A_v, B_v, &borrow);
        _mm256_storeu_si256(&(reinterpret_cast<__m256i_u *>(r)[i]), R_v);
    }

    a += max_avx_iters * 32 / sizeof(*a);
    b += max_avx_iters * 32 / sizeof(*b);
    r += max_avx_iters * 32 / sizeof(*r);

    const auto a_unaligned = (a_size - max_avx_iters * 32) / sizeof(*a);
    const auto b_unaligned = (b_size - max_avx_iters * 32) / sizeof(*b);

    std::size_t i;
    for (i = 0; i < b_unaligned; ++i)
    {
        const auto result = safe_sub(a[i], b[i], borrow);
        borrow = static_cast<uint32_t>(result >> (sizeof(borrow) * 8)) & 0x01;
        r[i] = static_cast<bigint_base_t>(result);
    }

    for (; i < a_unaligned; ++i)
    {
        const auto result = safe_sub(a[i], borrow);
        borrow = static_cast<uint32_t>(result >> (sizeof(borrow) * 8)) & 0x01;
        r[i] = static_cast<bigint_base_t>(result);
    }
}
#endif