#include <yabil/bigint/BigIntContext.h>

namespace yabil::bigint
{

std::unique_ptr<BigIntContext> BigIntContext::context = nullptr;

BigIntContext &BigIntContext::instance()
{
    if (context == nullptr)
    {
        std::unique_ptr<BigIntContext> created_instance(new BigIntContext());
        context = std::move(created_instance);
    }
    return *context;
}

ParallelismOption BigIntContext::get_parallelism() const
{
    return parallelism;
}

void BigIntContext::set_parallelism(ParallelismOption new_parallelism_option)
{
    parallelism = new_parallelism_option;
}

}  // namespace yabil::bigint
