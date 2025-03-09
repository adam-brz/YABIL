#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/utils/TypeUtils.h>

#include <string>

namespace yabil::bigint::impl
{

std::string to_string(const BigInt& number, const unsigned base);

}  // namespace yabil::bigint::impl
