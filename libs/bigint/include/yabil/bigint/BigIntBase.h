#pragma once

#include <yabil/bigint/algorithms_config.h>

namespace yabil::bigint
{

/// @brief Base type for big integer digit internal representation.
/// @details It can be configured using @p AlgorithmsDefaultsConfig.
/// In most cases default type is the most optimal one.
/// @headerfile BigIntBase.h <yabil/bigint/BigIntBase.h>
using bigint_base_t = AlgorithmsDefaultsConfig::bigint_base_t;

}  // namespace yabil::bigint
