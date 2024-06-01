#pragma once

#include <ranges>

namespace yabil::compile_time::detail
{

inline consteval auto normalize(const auto &number)
{
    return std::ranges::reverse_view(
        std::ranges::drop_while_view(std::ranges::reverse_view(number), [](const auto &digit) { return digit == 0; }));
}

}  // namespace yabil::compile_time::detail