#pragma once

#include <yabil/bigint/BigInt.h>

#include <span>

namespace yabil::bigint::parallel
{

BigInt parallel_add(const BigInt &a, const BigInt &b);

}