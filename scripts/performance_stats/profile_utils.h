#pragma once

#include <chrono>
#include <iostream>

template <typename Function, typename OutDuration = std::chrono::nanoseconds>
void profile_function(Function f, std::ostream &out, int start, int stop, int repeat = 1)
{
    for (int i = start; i < stop; ++i)
    {
        int64_t total = 0;
        for (int j = 0; j < repeat; ++j)
        {
            const auto start_time = std::chrono::steady_clock::now();
            f(i);
            const auto end_time = std::chrono::steady_clock::now();
            total += std::chrono::duration_cast<OutDuration>(end_time - start_time).count();
        }
        out << i << " " << total/repeat << std::endl;
    }
}
