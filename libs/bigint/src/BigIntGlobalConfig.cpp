#include <yabil/bigint/BigIntGlobalConfig.h>


namespace yabil::bigint
{

BigIntGlobalConfig& BigIntGlobalConfig::instance()
{
    static BigIntGlobalConfig config;
    return config;
}

void BigIntGlobalConfig::set_thread_count(int thread_count)
{
#if !YABIL_CONFIG_PARALLEL_DISABLED
    instance().number_of_threads = thread_count;
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
