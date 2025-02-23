#pragma once

#include <algorithm>
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

    const auto distance = std::distance(begin, end);
    if (distance == 0)
    {
        return std::span<begin_data_t>{};
    }

    return std::span<begin_data_t>(&(*begin), distance);
}

/// @brief Advance iterator and clamp advance amount if operation would result in out of bounds iterator.
/// @tparam Iterator Iterator type
/// @tparam Container Container of iterator type
/// @param iter Iterator
/// @param amount Advance value
/// @param container Container to which iterator belongs to
template <typename Iterator, typename Container>
constexpr auto safe_advance(Iterator iter, int amount, Container &container)
{
    if (amount < 0)
    {
        amount = std::max(-static_cast<int>(std::distance(container.begin(), iter)), amount);
    }
    else
    {
        amount = std::min(static_cast<int>(std::distance(iter, container.end())), amount);
    }
    return iter + amount;
}

}  // namespace yabil::utils
