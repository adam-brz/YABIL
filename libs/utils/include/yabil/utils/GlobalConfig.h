#pragma once

namespace yabil::utils
{

template <typename StoredConfig>
class GlobalConfig
{
public:
    StoredConfig config;

public:
    GlobalConfig(const GlobalConfig &) = delete;
    GlobalConfig(GlobalConfig &) = delete;
    GlobalConfig &operator=(const GlobalConfig &) = delete;

    /// @brief Get unique instance of configuration class.
    /// @return Reference to \p GlobalConfig object
    static inline GlobalConfig &instance()
    {
        static GlobalConfig<StoredConfig> unique_instance;
        return unique_instance;
    }

protected:
    GlobalConfig() = default;
};

}  // namespace yabil::utils
