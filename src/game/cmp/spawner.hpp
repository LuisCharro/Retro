#pragma once

#include <cstdint>
#include <chrono>

#include <ecs/cmp/component.hpp>

//namespace GAME {

using namespace std::chrono_literals; // Not good to use usings here

struct SpawnerComponent_t : public ECS::ComponentBase_t<SpawnerComponent_t>
{
    using clk =  std::chrono::steady_clock;

    explicit SpawnerComponent_t (ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {
    }

    clk::time_point last_spawn_time {std::chrono::steady_clock::now()};

    std::chrono::duration<double> spawn_interval {5s};

    std::size_t to_be_spawned {2};

};

//}