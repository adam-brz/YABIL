#include <cstdint>

namespace yabil::bigint
{

/// @brief Sign of big integer
/// @details Sign can be either "Plus" or "Minus"
enum class Sign : uint8_t
{
    Plus,
    Minus
};

}
