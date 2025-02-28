#pragma once

namespace yabil::utils
{

template <typename StoredType>
class GlobalInstance
{
private:
    StoredType v;

public:
    GlobalInstance(const GlobalInstance &) = delete;
    GlobalInstance(GlobalInstance &) = delete;
    GlobalInstance &operator=(const GlobalInstance &) = delete;

    /// @brief Get reference to unique instance.
    /// @return Reference to unique instance of \p StoredType type.
    static StoredType &instance()
    {
        static GlobalInstance<StoredType> unique_instance;
        return unique_instance.v;
    }

protected:
    GlobalInstance() = default;
};

}  // namespace yabil::utils
