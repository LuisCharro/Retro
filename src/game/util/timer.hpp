#pragma once

#include <chrono>
#include <thread>

struct GameTImer_t
{
    using clk           = std::chrono::steady_clock;
    using nano_t        = std::chrono::duration<uint64_t,std::nano>;
    using timepoint_t   = std::chrono::time_point<clk, nano_t>;

    explicit GameTImer_t() noexcept = default;

    void start() noexcept { m_start = clk::now(); }

    uint64_t ellapsed() noexcept 
    {
        auto ns_passed {clk::now() - m_start };

        return ns_passed.count();
    }

    uint64_t waitUntil_ns(uint64_t to_ns) 
    {
        if (ellapsed() < to_ns)
        {
            auto towait {to_ns - ellapsed()};
            std::this_thread::sleep_for(nano_t{towait});
            return towait;
        }
        
        return 0;
    }

    private:
        
        timepoint_t m_start;
};