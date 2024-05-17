#include <ranges>

namespace yabil::compile_time
{

inline consteval auto normalize(const auto &number)
{
    return std::ranges::take_while_view(number, [](const auto &digit) { return digit != 0; });
}

}  // namespace yabil::compile_time