#pragma once

#include <yabil/bigint/BigInt.h>

#include <memory>

#define BIGINT_CONTEXT_PARALLEL_BLOCK(...)                                        \
    {                                                                             \
        BigIntContext::instance().set_parallelism(ParallelismOption::Parallel);   \
        __VA_ARGS__;                                                              \
        BigIntContext::instance().set_parallelism(ParallelismOption::Sequential); \
    }

namespace yabil::bigint
{

enum class ParallelismOption
{
    Sequential,
    Parallel
};

/// @brief BigIntContext singleton
class BigIntContext
{
private:
    static std::unique_ptr<BigIntContext> context;
    ParallelismOption parallelism = ParallelismOption::Sequential;

public:
    /// @brief Get \p BigIntContext instance, creates new instance on first invocation.
    static BigIntContext& instance();

    /// @brief Get current parallelism option.
    /// @return Current parallelism option.
    ParallelismOption get_parallelism() const;

    /// @brief Set new parallelism option.
    /// @param new_parallelism_option New value op \p ParallelismOption
    void set_parallelism(ParallelismOption new_parallelism_option);

protected:
    BigIntContext() = default;
};

}  // namespace yabil::bigint
