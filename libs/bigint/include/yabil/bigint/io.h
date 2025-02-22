#pragma once

#include <yabil/bigint/BigInt.h>

#include <istream>
#include <ostream>

namespace yabil::bigint
{

std::ostream &operator<<(std::ostream &out, const BigInt &bigint);
std::istream &operator>>(std::istream &in, BigInt &bigint);

}  // namespace yabil::bigint