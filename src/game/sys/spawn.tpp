
//#include <util/gamecontext.hpp>

#include <iostream>


// Systems
#include "spawn.hpp"

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/spawner.hpp>

//namespace ECS {

    template<typename GameCTX_t>
    void 
    SpawnSystem_t<GameCTX_t>::SpawnGhost(GameCTX_t& g, uint32_t x, uint32_t y) const
    {
        auto& e  = g.CreateEntity();
        auto& rn = g.template AddComponent<RenderComponent_t>(e);
        auto& ph = g.template AddComponent<PhysicsComponent_t>(e);
        auto& cl = g.template AddComponent<ColliderComponent_t>(e);

        rn.transparency = true;

        rn.LoadFromFile("assets/fantasma.png");

        ph.x = x; ph.y = y;
        ph.vx = ph.vy = 3; 
        
        cl.box.xLeft = 0;
        cl.box.xRight = rn.w;
        cl.box.yUp = rn.h;
        cl.box.yDown = 0;
    }


    template<typename GameCTX_t>
    PhysicsComponent_t*
    SpawnSystem_t<GameCTX_t>::GetRequiredComponent(GameCTX_t& g, SpawnerComponent_t& spw) const
    {
        auto* e = g.GetEntityByID(spw.GetEntityID());

        if (e)
        {
            auto* ph = e->template getComponent<PhysicsComponent_t>();
            return ph;
        }

        return nullptr;
    }

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
            auto* phy = GetRequiredComponent(g, spw);

            if (!phy) continue;

            auto passed {now - spw.last_spawn_time};
            if (spw.to_be_spawned > 0 && passed > spw.spawn_interval)
            {
                SpawnGhost(g,phy->x,phy->y);
                spw.last_spawn_time = now;
                --spw.to_be_spawned;
            }
        }
    }
//}