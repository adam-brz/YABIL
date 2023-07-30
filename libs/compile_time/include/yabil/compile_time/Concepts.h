#include <concepts>
#include <type_traits>

namespace yabil::compile_time
{

template <typename T>
concept ConstBigIntType = requires(T)
{
    typename T::is_bigint;
};

}  // namespace yabil::compile_time