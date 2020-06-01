#pragma once

#include <cstdint>
#include <chrono>
#include <functional>

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

    //using PFun_t = void (*)(void);
    //using PFun_t = void (GameObjectFactory_t::*)(void);

    //void (*SpawnMethod)(void) {};
    //PFun_t SpawnMethod {};
        
    //Function is a class that encasuple a pointer to fucntion with type assur conversion 
    //std::function<void(void)> SpawnMethod {};
    std::function<void(const SpawnerComponent_t&)> SpawnMethod {};

    clk::time_point last_spawn_time {std::chrono::steady_clock::now()};

    std::chrono::duration<double> spawn_interval {5s};

    std::size_t to_be_spawned {2};
};

//}