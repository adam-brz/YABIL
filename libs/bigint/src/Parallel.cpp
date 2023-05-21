#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/Parallel.h>
#include <yabil/utils/ThreadPoolSingleton.h>

#include <algorithm>
#include <future>
#include <vector>

#include "Arithmetic.h"
#include "Thresholds.h"

namespace yabil::bigint::parallel
{

namespace
{

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> parallel_unbalanced_div(
    std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> parallel_recursive_div(
    std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);

std::vector<bigint_base_t> parallel_add_unsigned(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    const auto min_s = std::min(a.size(), b.size());
    if (min_s < thresholds::parallel_add_digits)
    {
        return plain_add(a, b);
    }

    auto &thread_pool = utils::ThreadPoolSingleton::instance();
    auto concurrency = thread_pool.thread_count();
    if (min_s < concurrency)
    {
        concurrency = min_s;
    }

    const auto chunk_size = min_s / concurrency;

    std::vector<std::future<std::vector<bigint_base_t>>> partial_results;
    partial_results.reserve(concurrency);

    for (int i = 0; i < static_cast<int>(concurrency); ++i)
    {
        partial_results.push_back(thread_pool.submit(
            [&, i]()
            {
                return plain_add(
                    {a.begin() + static_cast<int>(i * chunk_size), a.begin() + static_cast<int>((i + 1) * chunk_size)},
                    {b.begin() + static_cast<int>(i * chunk_size), b.begin() + static_cast<int>((i + 1) * chunk_size)});
            }));
    }

    auto last_part = thread_pool.submit(
        [&]()
        {
            return plain_add({a.begin() + static_cast<int>(concurrency * chunk_size), a.end()},
                             {b.begin() + static_cast<int>(concurrency * chunk_size), b.end()});
        });

    std::vector<bigint_base_t> result(std::max(a.size(), b.size()) + 1);
    bigint_base_t carry = 0;

    int chunk_index = 0;
    for (auto &chunk : partial_results)
    {
        auto part_data = chunk.get();
        if (carry)
        {
            increment_unsigned(part_data);
            carry = 0;
        }
        if (part_data.size() > chunk_size)
        {
            carry += part_data.back();
        }
        std::copy(part_data.cbegin(), part_data.cbegin() + static_cast<int>(chunk_size), result.begin() + chunk_index);
        chunk_index += static_cast<int>(chunk_size);
    }

    auto final_part_data = last_part.get();
    if (carry)
    {
        increment_unsigned(final_part_data);
    }

    std::copy(final_part_data.cbegin(), final_part_data.cend(), result.begin() + chunk_index);
    return result;
}

std::vector<bigint_base_t> parallel_karatsuba(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    if (a.size() < thresholds::karatsuba_threshold_digits || b.size() < thresholds::karatsuba_threshold_digits)
    {
        return mul_basecase(a, b);
    }

    const int m2 = static_cast<int>(std::max(a.size(), b.size()) / 2);

    const std::span<bigint_base_t const> low1{a.begin(), a.begin() + m2};
    const std::span<bigint_base_t const> high1{a.begin() + m2, a.end()};

    const std::span<bigint_base_t const> low2{b.begin(), b.begin() + m2};
    const std::span<bigint_base_t const> high2{b.begin() + m2, b.end()};

    auto &thread_pool = utils::ThreadPoolSingleton::instance();

    auto w_z0 = thread_pool.submit_run_task([&]() { return parallel_karatsuba(low1, low2); });
    auto w_z1 = thread_pool.submit_run_task(
        [&]()
        {
            const auto lh1 = plain_add(low1, high1);
            const auto lh2 = plain_add(low2, high2);
            return parallel_karatsuba(lh1, lh2);
        });
    auto w_z2 = thread_pool.submit_run_task([&]() { return parallel_karatsuba(high1, high2); });

    const auto z0 = BigInt(w_z0.get());
    const auto z1 = BigInt(w_z1.get());
    const auto z2 = BigInt(w_z2.get());

    auto result =
        (z2 << (m2 * 2UL * sizeof(bigint_base_t) * 8UL)) + ((z1 - z2 - z0) << (m2 * sizeof(bigint_base_t) * 8UL)) + z0;
    return result.raw_data();
}

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> parallel_divide_unsigned(
    std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    if (a.size() > thresholds::recursive_div_threshold_digits)
    {
        return parallel_unbalanced_div(a, b);
    }

    const auto [q, r] = BigInt(a).base_div(BigInt(b));
    return {q.raw_data(), r.raw_data()};
}

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> parallel_unbalanced_div(
    std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    constexpr auto digit_size_bits = sizeof(bigint_base_t) * 8;
    const int n = static_cast<int>(b.size());
    int m = static_cast<int>(a.size()) - n;

    std::vector<bigint_base_t> A;
    BigInt Q;

    while (m > n)
    {
        const auto [q, r] = parallel_recursive_div(a.subspan(m - n), b);
        Q = (Q << (digit_size_bits * n)) + BigInt(q);

        const auto R = BigInt(r) << (digit_size_bits * (m - n));
        A = plain_add(R.raw_data(), a.subspan(0, m - n));

        m -= n;
    }
    const auto [q, r] = parallel_recursive_div(A, b);
    const auto quotient = (Q << (digit_size_bits * m)) + BigInt(q);
    return {quotient.raw_data(), r};
}

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> parallel_recursive_div(
    std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    const int n =
        static_cast<int>(std::find_if(b.rbegin(), b.rend(), [](const auto &v) { return v != 0; }).base() - b.begin());
    const int m =
        static_cast<int>(std::find_if(a.rbegin(), a.rend(), [](const auto &v) { return v != 0; }).base() - a.begin()) -
        n;

    if (m < static_cast<int>(thresholds::recursive_div_threshold_digits))
    {
        const auto [q, r] = BigInt(a).divide(BigInt(b));
        return {q.raw_data(), r.raw_data()};
    }

    const int k = m / 2;
    constexpr auto digit_size_bits = sizeof(bigint_base_t) * 8;

    const auto B1 = b.subspan(k);
    const auto B0 = b.subspan(0, k);

    auto [Q1, R1] = parallel_recursive_div(a.subspan(2UL * k), B1);
    auto A_prim = BigInt(plain_add((BigInt(R1) << (digit_size_bits * 2 * k)).raw_data(), a.subspan(0, 2UL * k))) -
                  (BigInt(parallel_karatsuba(Q1, B0)) << (digit_size_bits * k));

    while (A_prim.is_negative())
    {
        decrement_unsigned(Q1);
        A_prim += BigInt(b) << (digit_size_bits * k);
    }

    std::span<bigint_base_t const> A_prim_view(A_prim.raw_data());

    auto [Q0, R0] = parallel_recursive_div(A_prim_view.subspan(k), B1);
    auto A_bis = BigInt(plain_add((BigInt(R0) << (digit_size_bits * k)).raw_data(), A_prim_view.subspan(0, k))) -
                 BigInt(parallel_karatsuba(Q0, B0));

    while (A_bis.is_negative())
    {
        decrement_unsigned(Q0);
        A_bis += BigInt(b);
    }

    return {plain_add((BigInt(Q1) << (digit_size_bits * k)).raw_data(), Q0), A_bis.raw_data()};
}

}  // namespace

BigInt add(const BigInt &a, const BigInt &b)
{
    if (a.get_sign() == b.get_sign())
    {
        return BigInt(parallel_add_unsigned(a.raw_data(), b.raw_data()), a.get_sign());
    }

    const auto [greater, lower] = get_greater_lower(a, b);
    const Sign new_sign = ((greater == &a) == (a.get_sign() == Sign::Plus)) ? Sign::Plus : Sign::Minus;
    return BigInt(plain_sub(greater->raw_data(), lower->raw_data()), new_sign);
}

BigInt multiply(const BigInt &a, const BigInt &b)
{
    return BigInt(parallel_karatsuba(a.raw_data(), b.raw_data()),
                  (a.get_sign() == b.get_sign()) ? Sign::Plus : Sign::Minus);
}

std::pair<BigInt, BigInt> divide(const BigInt &a, const BigInt &b)
{
    if (b.is_zero())
    {
        throw std::invalid_argument("Cannot divide by 0");
    }

    if (a.raw_data().size() < thresholds::parallel_div_digits || b.raw_data().size() < thresholds::parallel_div_digits)
    {
        return a.divide(b);
    }

    if (!is_normalized_for_division(b))
    {
        const auto k = std::countl_zero(b.raw_data().back());
        const auto [quotient, remainder] = (a << k).divide(b << k);
        return {quotient, remainder >> k};
    }

    const auto [quotient, remainder] = parallel_divide_unsigned(a.raw_data(), b.raw_data());
    const Sign sign_quotient = (a.get_sign() == b.get_sign()) ? Sign::Plus : Sign::Minus;
    const Sign sign_remainder = a.get_sign();

    return {BigInt(quotient, sign_quotient), BigInt(remainder, sign_remainder)};
}

}  // namespace yabil::bigint::parallel
