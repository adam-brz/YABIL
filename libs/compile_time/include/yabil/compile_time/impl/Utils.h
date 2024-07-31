#pragma once

#include <yabil/compile_time/detail/BigIntData.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>

namespace yabil::compile_time::impl
{

template <std::size_t DataSize>
constexpr bigint_base_t get_digit(const std::size_t idx, BigIntData<DataSize> data)
{
    if (idx < DataSize)
    {
        return data[idx];
    }
    return 0;
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
    if constexpr (new_size == 0)
    {
        return BigIntData<1>{0};
    }
    else
    {
        BigIntData<new_size> normalized;
        std::copy_n(Data.cbegin(), new_size, normalized.begin());
        return normalized;
    }
}

}  // namespace yabil::compile_time::impl
