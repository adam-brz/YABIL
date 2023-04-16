#pragma once

#include <functional>
#include <memory>
#include <utility>

namespace yabil::utils
{

/// @brief Universal function wrapper class for functions with no arguments
class FunctionWrapper
{
private:
    struct ImplBase
    {
        virtual void call() = 0;
        virtual ~ImplBase() {}
    };

    template <typename F>
    struct ImplType : ImplBase
    {
    private:
        F f;

    public:
        explicit ImplType(F &&function) : f(std::forward<F>(function)) {}
        void call() override
        {
            std::invoke(f);
        }
    };

private:
    std::unique_ptr<ImplBase> impl;

public:
    /// @brief Creates empty function wrapper
    FunctionWrapper();

    /// @brief Move constructor
    /// @param other \p FunctionWrapper
    FunctionWrapper(FunctionWrapper &&other) noexcept;

    FunctionWrapper(const FunctionWrapper &) = delete;
    FunctionWrapper(FunctionWrapper &) = delete;

    /// @brief Creates function wrapper for specified function
    /// @tparam F function type
    /// @param f function
    template <typename F>
    explicit FunctionWrapper(F &&f) : impl(new ImplType<F>(std::forward<F>(f)))
    {
    }

    /// @brief Invoke underlying function
    void call() const;

    FunctionWrapper &operator=(FunctionWrapper &&other) noexcept;
    FunctionWrapper &operator=(const FunctionWrapper &) = delete;
};

}  // namespace yabil::utils
