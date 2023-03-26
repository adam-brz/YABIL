#include <yabil/crypto/Utils.h>

#include <bit>
#include <cmath>
#include <cstring>
#include <random>
#include <stdexcept>
#include <vector>

namespace yabil::crypto::utils
{

yabil::bigint::BigInt factorial(uint64_t n)
{
    yabil::bigint::BigInt result(1U);
    for (uint64_t i = 1; i <= n; ++i)
    {
        result *= yabil::bigint::BigInt(i);
    }
    return result;
}

uint64_t log2_int(const yabil::bigint::BigInt &number)
{
    if (number.is_negative() || number.is_zero())
    {
        throw std::invalid_argument("Logarithm argument must be greater than 0");
    }

    constexpr auto item_size_bits = sizeof(number.raw_data().front()) * 8;
    const auto bit_size = (number.raw_data().size() - 1) * item_size_bits;
    const auto last_item = number.raw_data().back();
    const auto last_one_pos = item_size_bits - std::countl_zero(last_item);
    return bit_size + last_one_pos - 1;
}

double log2(const yabil::bigint::BigInt &number)
{
    constexpr auto item_size_bits = sizeof(yabil::bigint::bigint_base_t) * 8;
    constexpr int result_iter_count = 64 / item_size_bits;

    const auto bit_shift_quotient_removal = std::countl_zero(number.raw_data().back()) + 1;
    uint64_t raw_fraction = 0;

    int i;
    for (i = 0; i < result_iter_count && i < static_cast<int>(number.raw_data().size()); ++i)
    {
        const uint64_t fraction_part = static_cast<uint64_t>(number.raw_data()[number.raw_data().size() - 1 - i])
                                       << bit_shift_quotient_removal;
        raw_fraction |= fraction_part << (64 - (i + 1) * item_size_bits);
    }

    if (i < static_cast<int>(number.raw_data().size()))
    {
        raw_fraction |= static_cast<uint64_t>(number.raw_data()[number.raw_data().size() - 1 - i]) >>
                        (item_size_bits - bit_shift_quotient_removal);
    }

    raw_fraction = (raw_fraction >> 12) | 0x3ff0000000000000;
    const double remaining_part = *static_cast<double *>(reinterpret_cast<void *>(&raw_fraction));
    return static_cast<double>(log2_int(number)) + std::log2(remaining_part);
}

double log(const yabil::bigint::BigInt &number, const yabil::bigint::BigInt &base)
{
    return log2(number) / log2(base);
}

double log(const yabil::bigint::BigInt &number, double base)
{
    return log2(number) / std::log2(base);
}

yabil::bigint::BigInt random_bigint(uint64_t number_of_bits)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::bernoulli_distribution dist(0.5);
    std::vector<yabil::bigint::bigint_base_t> raw_bigint_data;

    constexpr size_t chunk_size = sizeof(yabil::bigint::bigint_base_t) * 8;
    const size_t bigint_chunks_count = number_of_bits / chunk_size + 1;
    raw_bigint_data.reserve(bigint_chunks_count);

    size_t total_bits = 0;
    for (size_t i = 0; i < bigint_chunks_count; i++)
    {
        yabil::bigint::bigint_base_t chunk = 0;
        for (size_t j = 0; j < chunk_size && total_bits < number_of_bits; ++j)
        {
            chunk |= static_cast<yabil::bigint::bigint_base_t>(dist(gen)) << j;
            ++total_bits;
        }
        raw_bigint_data.push_back(chunk);
    }

    return yabil::bigint::BigInt(std::move(raw_bigint_data));
}

}  // namespace yabil::crypto::utils
