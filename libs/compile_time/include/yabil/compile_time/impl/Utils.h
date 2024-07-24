#pragma once

#include <yabil/compile_time/BigIntData.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>

namespace yabil::compile_time::impl
{

template <std::size_t DataSize>
consteval uint64_t get_digit(const int idx, BigIntData<DataSize> data)
{
    if (idx < DataSize)
    {
        return data[idx];
    }
    return 0;
}

inline consteval auto normalize(const auto &number_data)
{
    return std::ranges::reverse_view(std::ranges::drop_while_view(std::ranges::reverse_view(number_data),
                                                                  [](const auto &digit) { return digit == 0; }));
}

template <std::size_t DataSize, BigIntData<DataSize> Data>
inline consteval auto normalized_size()
{
    std::size_t size = DataSize;
    for (auto i = static_cast<int64_t>(DataSize - 1); i >= 0; --i)
    {
        if (Data[i] != 0)
        {
            break;
        }
        --size;
    }
    return size;
}

template <std::size_t DataSize, BigIntData<DataSize> Data>
inline consteval auto normalize()
{
    constexpr auto new_size = normalized_size<DataSize, Data>();
    BigIntData<new_size> normalized;
    std::copy_n(Data.cbegin(), new_size, normalized.begin());
    return normalized;
}

}  // namespace yabil::compile_time::impl