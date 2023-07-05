#pragma once

#include <span>
#include <type_traits>

namespace yabil::utils
{

/// @brief Make std::span from range described by two iterators.
/// @tparam BeginIter Begin iterator type
/// @tparam EndIter End iterator type
/// @param begin Begin iterator
/// @param end End iterator
/// @return std::span view over range between begin and end
template <typename BeginIter, typename EndIter>
constexpr auto make_span(BeginIter begin, EndIter end)
{
    using begin_data_t = std::remove_pointer_t<typename BeginIter::pointer>;
    using end_data_t = std::remove_pointer_t<typename EndIter::pointer>;

    static_assert(std::is_same_v<begin_data_t, end_data_t>,
                  "Both begin and end operators must point to data of the same type.");

    if (std::distance(begin, end) == 0)
    {
        return std::span<begin_data_t>{};
    }

    return std::span<begin_data_t>(&(*begin), std::distance(begin, end));
}

}  // namespace yabil::utils
