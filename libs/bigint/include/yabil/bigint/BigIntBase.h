#pragma once

#include <cstdint>

#ifndef BIGINT_BASE_T
#ifdef __SIZEOF_INT128__
#define BIGINT_BASE_T uint64_t
#else
#define BIGINT_BASE_T uint32_t
#endif
#endif

namespace yabil
{

namespace bigint
{
/// @brief Base type for big integer internal representation
using bigint_base_t = BIGINT_BASE_T;

}  // namespace bigint

}  // namespace yabil