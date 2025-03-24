#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/arithmetic/ArrayAddSub.h>

#include <cassert>
#include <cstdint>
#include <type_traits>

#if defined(YABIL_INTRINSICS_HEADER_FILE)
#if __has_include(YABIL_INTRINSICS_HEADER_FILE)
#include YABIL_INTRINSICS_HEADER_FILE
#define ADD_SUB_HAS_IMMINTRIN
#endif
#endif

namespace yabil::bigint::arithmetic
{

namespace
{

#ifdef ADD_SUB_HAS_IMMINTRIN
static constexpr bool has_intrinsics = true;
#else
static constexpr bool has_intrinsics = false;
#endif

template <bool has_intrinsics>
struct GenericArithProvider
{
#ifdef ADD_SUB_HAS_IMMINTRIN
    template <typename bigint_t = bigint_base_t>
    static std::enable_if_t<sizeof(bigint_t) == 8, bigint_base_t> add_carry(const bigint_base_t carry,
                                                                            const bigint_base_t a,
                                                                            const bigint_base_t b,
                                                                            bigint_base_t *destination)
    {
        using op_t = unsigned long long; // NOLINT
        return _addcarry_u64(static_cast<uint8_t>(carry), static_cast<op_t>(a), static_cast<op_t>(b),
                             reinterpret_cast<op_t *>(destination));
    }

    template <typename bigint_t = bigint_base_t>
    static std::enable_if_t<sizeof(bigint_t) == 4, bigint_base_t> add_carry(const bigint_base_t carry,
                                                                            const bigint_base_t a,
                                                                            const bigint_base_t b,
                                                                            bigint_base_t *destination)
    {
        using op_t = unsigned int; // NOLINT
        return _addcarry_u32(static_cast<uint8_t>(carry), static_cast<op_t>(a), static_cast<op_t>(b),
                             reinterpret_cast<op_t *>(destination));
    }

    template <typename bigint_t = bigint_base_t>
    static std::enable_if_t<sizeof(bigint_t) == 8, bigint_base_t> sub_borrow(const bigint_base_t borrow,
                                                                             const bigint_base_t a,
                                                                             const bigint_base_t b,
                                                                             bigint_base_t *destination)
    {
        using op_t = unsigned long long; // NOLINT
        return _subborrow_u64(static_cast<uint8_t>(borrow), static_cast<op_t>(a), static_cast<op_t>(b),
                              reinterpret_cast<op_t *>(destination));  // NOLINT
    }

    template <typename bigint_t = bigint_base_t>
    static std::enable_if_t<sizeof(bigint_t) == 4, bigint_base_t> sub_borrow(const bigint_base_t borrow,
                                                                             const bigint_base_t a,
                                                                             const bigint_base_t b,
                                                                             bigint_base_t *destination)
    {
        using op_t = unsigned int; // NOLINT
        return _subborrow_u32(static_cast<uint8_t>(borrow), static_cast<op_t>(a), static_cast<op_t>(b),
                              reinterpret_cast<op_t *>(destination));
    }
#endif

    template <typename bigint_t = bigint_base_t>
    static std::enable_if_t<(!has_intrinsics || sizeof(bigint_t) > 8 || sizeof(bigint_t) < 4), bigint_base_t> add_carry(
        const bigint_base_t carry, const bigint_base_t a, const bigint_base_t b, bigint_base_t *destination)

    {
        return add_carry_simple(carry, a, b, destination);
    }

    template <typename bigint_t = bigint_base_t>
    static std::enable_if_t<(!has_intrinsics || sizeof(bigint_t) > 8 || sizeof(bigint_t) < 4), bigint_base_t>
    sub_borrow(const bigint_base_t borrow, const bigint_base_t a, const bigint_base_t b, bigint_base_t *destination)
    {
        return sub_borrow_simple(borrow, a, b, destination);
    }

    static bigint_base_t add_carry_simple(const bigint_base_t carry, const bigint_base_t a, const bigint_base_t b,
                                          bigint_base_t *destination)
    {
        const bigint_base_t tmp1 = a + carry;
        bigint_base_t carry_out = static_cast<bigint_base_t>(tmp1 < carry);
        const bigint_base_t tmp2 = (tmp1 + b);
        carry_out += static_cast<bigint_base_t>(tmp2 < tmp1);
        *destination = tmp2;
        return carry_out;
    }

    static bigint_base_t sub_borrow_simple(const bigint_base_t borrow, const bigint_base_t a, const bigint_base_t b,
                                           bigint_base_t *destination)
    {
        *destination = (a - b - borrow);
        if (a != b)
        {
            return static_cast<bigint_base_t>(a < b);
        }
        return borrow;
    }
};

using ArithProvider = GenericArithProvider<has_intrinsics>;

}  // namespace

void add_arrays_with_carry(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                           bigint_base_t *r, bigint_base_t carry)
{
    assert(a_size >= b_size);

    std::size_t i;
    for (i = 0; i < b_size; ++i)
    {
        carry = ArithProvider::add_carry(carry, a[i], b[i], &r[i]);
    }

    for (; i < a_size; ++i)
    {
        carry = ArithProvider::add_carry(carry, a[i], 0, &r[i]);
    }

    if (carry)
    {
        r[i] = carry;
    }
}

void sub_arrays_with_borrow(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                            bigint_base_t *r, bigint_base_t borrow)
{
    assert(a_size >= b_size);

    std::size_t i;
    for (i = 0; i < b_size; ++i)
    {
        borrow = ArithProvider::sub_borrow(borrow, a[i], b[i], &r[i]);
    }

    for (; i < a_size; ++i)
    {
        borrow = ArithProvider::sub_borrow(borrow, a[i], 0, &r[i]);
    }
}

}  // namespace yabil::bigint::arithmetic
