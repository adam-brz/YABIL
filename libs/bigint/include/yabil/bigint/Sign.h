#pragma once

#include <cstdint>

namespace yabil::bigint
{

/// @brief Sign of big integer. Can be either @p Sign::Plus or @p Sign::Minus
enum class Sign : uint8_t
{
    Plus,
    Minus
};

}  // namespace yabil::bigint
