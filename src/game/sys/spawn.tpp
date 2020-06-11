
//#include <util/gamecontext.hpp>

#include <iostream>


// Systems
#include "spawn.hpp"

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/spawner.hpp>

template<typename GameCTX_t>
void 
SpawnSystem_t<GameCTX_t>::Update(GameCTX_t& g) const
{
    //using clk = std::chrono::steady_clock;
    //auto now = std::chrono::steady_clock::now();

    using namespace std::chrono;
    auto now = steady_clock::now();

    for (auto& spw : g.template GetComponents<SpawnerComponent_t>()) 
    {
        auto* phy = g.template GetRequiredComponent<PhysicsComponent_t>(spw);

        if (!phy) continue;

        auto passed {now - spw.last_spawn_time};
        if (spw.to_be_spawned > 0 && passed > spw.spawn_interval)
        {
            spw.SpawnMethod(spw);

            spw.last_spawn_time = now;
            --spw.to_be_spawned;
        }
    }
}