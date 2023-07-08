#pragma once

#include <atomic>
#include <memory>

namespace yabil::bigint
{

class BigIntGlobalConfig
{
private:
    std::atomic<int> number_of_threads;
    std::atomic<bool> parallel_algorithms_enabled = true;

public:
    BigIntGlobalConfig(const BigIntGlobalConfig &) = delete;
    BigIntGlobalConfig(BigIntGlobalConfig &) = delete;
    BigIntGlobalConfig &operator=(const BigIntGlobalConfig &) = delete;

    static BigIntGlobalConfig &instance();

    void set_thread_count(int thread_count);
    void set_parallel_algorithms_enabled(bool parallel_algorithms_enabled);

    bool use_parallel_algorithms() const;
    int get_number_of_threads() const;

protected:
    BigIntGlobalConfig();
};

}  // namespace yabil::bigint
