#pragma once

#include <yabil/compile_time/detail/BigIntData.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>

namespace yabil::compile_time::impl
{

template <std::size_t DataSize>
constexpr bigint_base_t get_digit(const std::size_t idx, const BigIntData<DataSize> &data)
{
    if (idx < DataSize)
    {
        return data[idx];
    }
    return 0;
}

template <typename DataType, std::size_t DataSize, std::array<DataType, DataSize> Data>
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

template <typename DataType, std::size_t DataSize, std::array<DataType, DataSize> Data>
inline consteval auto normalize()
{
    constexpr auto new_size = normalized_size<DataType, DataSize, Data>();
    if constexpr (new_size == 0)
    {
        return BigIntData<1>{0};
    }
    else
    {
        std::array<DataType, new_size> normalized;
        std::copy_n(Data.cbegin(), new_size, normalized.begin());
        return normalized;
    }
}

template <std::size_t DataSize, BigIntData<DataSize> Data>
inline consteval auto normalized_size()
{
    return normalized_size<bigint_base_t, DataSize, Data>();
}

template <std::size_t DataSize, BigIntData<DataSize> Data>
inline consteval auto normalize()
{
    return normalize<bigint_base_t, DataSize, Data>();
}

}  // namespace yabil::compile_time::impl
