#pragma once

#include <yabil/bigint/BigInt.h>

#include <istream>
#include <ostream>

namespace yabil::bigint
{

/// @brief Overload of stream write operator.
/// @headerfile io.h <yabil/bigint/io.h>
std::ostream &operator<<(std::ostream &out, const BigInt &bigint);

/// @brief Overload of stream read operator.
/// @headerfile io.h <yabil/bigint/io.h>
std::istream &operator>>(std::istream &in, BigInt &bigint);

}  // namespace yabil::bigint
