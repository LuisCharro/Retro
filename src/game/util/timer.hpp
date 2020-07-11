#pragma once

#include <chrono>
#include <thread>

struct GameTimer_t
{
    using clk           = std::chrono::steady_clock;
    using nano_t        = std::chrono::duration<uint64_t,std::nano>;
    using timepoint_t   = std::chrono::time_point<clk, nano_t>;

    explicit GameTimer_t() noexcept 
    {
        start();
    }

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

 auto timeCall = [](std::string_view name, auto func){
    GameTimer_t internalTimer;
    func();    
    std::cout << "[" << name << "] " << std::setw(6) << std::setfill('0') << internalTimer.ellapsed() / 1000 << " ";
};