#include <yabil/bigint/BigInt.h>
#include <yabil/parallel/Parallel.h>

#include <tuple>

#include "ParallelCPUInterface.h"
#include "gpu/ParallelGPUInterface.h"
#include "yabil/bigint/BigIntBase.h"
#include "yabil/bigint/algorithms_config.h"
#include "yabil/parallel/ParallelGlobalConfig.h"

namespace yabil::parallel
{

namespace
{

std::vector<bigint::bigint_base_t> add_unsigned(const std::span<const bigint::bigint_base_t> &a,
                                                const std::span<const bigint::bigint_base_t> &b)
{
    if constexpr (bigint::AlgorithmsDefaultsConfig::with_cuda)
    {
        // Enable only if we can compile with CUDA and it was enabled in config
        const auto &config = ParallelGlobalConfig::instance();
        if (config.enable_cuda && config.cuda_add_threshold < a.size() && config.cuda_add_threshold < b.size())
        {
            const auto &[x, y] = a.size() >= b.size() ? std::tie(a, b) : std::tie(b, a);
            std::vector<bigint::bigint_base_t> ret(x.size() + 1);
            gpu::add_arrays(x.data(), x.size(), y.data(), y.size(), ret.data());
            return ret;
        }
    }
    return parallel_add_unsigned(a, b);
}

}  // namespace

bigint::BigInt add(const bigint::BigInt &a, const bigint::BigInt &b)
{
    if (a.get_sign() == b.get_sign())
    {
        return bigint::BigInt(add_unsigned(a.raw_data(), b.raw_data()), a.get_sign());
    }

    // TODO(.): Implement subtraction, add benchmarks
    return a + b;
}

bigint::BigInt mul(const bigint::BigInt &a, const bigint::BigInt &b)
{
    return bigint::BigInt(parallel_karatsuba(a.raw_data(), b.raw_data()),
                          (a.get_sign() == b.get_sign()) ? bigint::Sign::Plus : bigint::Sign::Minus);
}

}  // namespace yabil::parallel
