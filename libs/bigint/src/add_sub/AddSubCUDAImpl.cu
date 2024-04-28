#include <cuda_runtime.h>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>

#include "AddSub.h"

namespace yabil
{

namespace bigint
{

constexpr int device_add_block_size = 64;

__global__ void add_with_carry(const bigint_base_t *a, const bigint_base_t *b, bigint_base_t *r, bigint_base_t *c)
{
    const int idx_c = blockDim.x * blockIdx.x + threadIdx.x;
    const int idx_r = idx_c * device_add_block_size;
    bigint_base_t carry = 0;

    for (int i = idx_r; i < idx_r + device_add_block_size; ++i)
    {
        const bigint_base_t tmp1 = a[i] + carry;
        carry = static_cast<bigint_base_t>(tmp1 < carry);
        const bigint_base_t tmp2 = (tmp1 + b[i]);
        carry += static_cast<bigint_base_t>(tmp2 < tmp1);
        r[i] = tmp2;
    }

    c[idx_c] = carry;
}

void add_arrays(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
                std::size_t b_size, yabil::bigint::bigint_base_t *r)
{
    assert(a_size >= b_size);

    constexpr int max_thread_count = 256;
    const auto max_common_size = static_cast<unsigned>(b_size);

    const auto blocks_count = max_common_size / max_thread_count;
    const auto results_output_size = blocks_count * max_thread_count;
    bigint_base_t carry = 0;

    if (blocks_count > 0)
    {
        const auto carry_output_size = results_output_size / device_add_block_size;
        bigint_base_t *carries = new bigint_base_t[carry_output_size];

        {
            bigint_base_t *a_device, *b_device, *results_device, *carries_device;
            cudaMalloc(&a_device, results_output_size);
            cudaMalloc(&b_device, results_output_size);
            cudaMalloc(&results_device, results_output_size + 1);
            cudaMalloc(&carries_device, carry_output_size);

            cudaMemcpy(a_device, a, results_output_size, cudaMemcpyHostToDevice);
            cudaMemcpy(b_device, b, results_output_size, cudaMemcpyHostToDevice);

            add_with_carry<<<blocks_count, max_thread_count>>>(a_device, b_device, results_device, carries_device);

            cudaMemcpy(r, results_device, results_output_size, cudaMemcpyDeviceToHost);
            cudaMemcpy(carries, carries_device, carry_output_size, cudaMemcpyDeviceToHost);

            cudaFree(a_device);
            cudaFree(b_device);
            cudaFree(results_device);
            cudaFree(carries_device);
        }

        for (unsigned i = 0; i < carry_output_size; ++i)
        {
            carry += carries[i];
            if (carry == 0)
            {
                continue;
            }

            for (unsigned j = (i + 1) * device_add_block_size; j < (i + 2) * device_add_block_size; ++j)
            {
                r[j] += 1;
                if (r[j] != 0)
                {
                    carry = 0;
                    break;
                }
            }
        }
        r[results_output_size] = carry;
        delete[] carries;
    }

    add_plain_arrays(&a[results_output_size], a_size - results_output_size, &b[results_output_size],
                     b_size - results_output_size, &r[results_output_size], carry);
}

void sub_arrays(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
                std::size_t b_size, yabil::bigint::bigint_base_t *r)
{
    sub_plain_arrays(a, a_size, b, b_size, r);
}
}  // namespace bigint

}  // namespace yabil
