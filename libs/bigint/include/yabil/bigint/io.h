#pragma once

#include <yabil/bigint/BigInt.h>

#include <istream>
#include <ostream>

namespace yabil::bigint
{

/// @brief Overload of stream write operator.
/// Converts big integer to string in base 10 representation.
/// @headerfile io.h <yabil/bigint/io.h>
std::ostream &operator<<(std::ostream &out, const BigInt &bigint);

/// @brief Overload of stream read operator.
/// Reads from stream and loads big integer from string in base 10 representation.
/// @headerfile io.h <yabil/bigint/io.h>
std::istream &operator>>(std::istream &in, BigInt &bigint);

}  // namespace yabil::bigint
