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
#if !YABIL_CONFIG_PARALLEL_DISABLED
    instance().number_of_threads = thread_count;
    parallel::set_thread_count(static_cast<std::size_t>(thread_count));
#endif
}

int BigIntGlobalConfig::get_thread_count()
{
#if !YABIL_CONFIG_PARALLEL_DISABLED
    return instance().number_of_threads;
#else
    return 1;
#endif
}

}  // namespace yabil::bigint
