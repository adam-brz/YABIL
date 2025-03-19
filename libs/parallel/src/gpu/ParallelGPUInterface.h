#pragma once

#include <yabil/bigint/BigIntBase.h>



namespace yabil::parallel::gpu
{

void add_arrays(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
                std::size_t b_size, yabil::bigint::bigint_base_t *r);

}
