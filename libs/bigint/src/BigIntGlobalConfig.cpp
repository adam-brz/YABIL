#include <yabil/bigint/BigIntGlobalConfig.h>

#include "parallel/ParallelImpl.h"

namespace yabil::bigint
{

BigIntGlobalConfig::BigIntGlobalConfig() : number_of_threads(static_cast<int>(parallel::get_thread_count())) {}

BigIntGlobalConfig& BigIntGlobalConfig::instance()
{
    static BigIntGlobalConfig config;
    return config;
}

void BigIntGlobalConfig::set_thread_count(int thread_count)
{
    number_of_threads = thread_count;
    parallel::set_thread_count(static_cast<std::size_t>(thread_count));
}

void BigIntGlobalConfig::set_parallel_algorithms_enabled(bool enabled)
{
    parallel_algorithms_enabled = enabled;
}

bool BigIntGlobalConfig::use_parallel_algorithms() const
{
    return parallel_algorithms_enabled;
}

int BigIntGlobalConfig::get_number_of_threads() const
{
    return number_of_threads;
}

const Thresholds& BigIntGlobalConfig::thresholds() const
{
    return bigint_thresholds;
}

void BigIntGlobalConfig::set_thresholds(const Thresholds& new_thresholds)  // cppcheck-suppress [unusedFunction]
{
    bigint_thresholds = new_thresholds;
}

}  // namespace yabil::bigint
