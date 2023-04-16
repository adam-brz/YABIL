#include <yabil/utils/FunctionWrapper.h>

namespace yabil::utils
{

FunctionWrapper::FunctionWrapper(FunctionWrapper &&other) noexcept : impl(std::move(other.impl)) {}

FunctionWrapper::FunctionWrapper() : impl(new ImplType<std::function<void()>>([]() {})) {}

void FunctionWrapper::call() const
{
    impl->call();
}

FunctionWrapper &FunctionWrapper::operator=(FunctionWrapper &&other) noexcept
{
    impl = std::move(other.impl);
    return *this;
}

}  // namespace yabil::utils
