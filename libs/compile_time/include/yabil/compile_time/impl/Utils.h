#pragma once

#include <yabil/compile_time/BigIntData.h>

#include <ranges>

namespace yabil::compile_time::impl
{

inline consteval auto normalize(const auto &number)
{
    return std::ranges::reverse_view(
        std::ranges::drop_while_view(std::ranges::reverse_view(number), [](const auto &digit) { return digit == 0; }));
}

template <std::size_t DataSize>
consteval uint64_t get_digit(const int idx, BigIntData<DataSize> data)
{
    if (idx < DataSize)
    {
        return data[idx];
    }
    return 0;
}

}  // namespace yabil::compile_time::impl