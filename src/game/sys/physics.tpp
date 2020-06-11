
// Systems
#include "physics.hpp"

#include <ecs/cmp/entity.hpp>

// Components
#include <game/cmp/physics.hpp>

template<typename GameCTX_t>
bool 
PhysicsSystem_t<GameCTX_t>::Update(GameCTX_t& g) const
{
    for (auto& phy : g.template GetComponents<PhysicsComponent_t>()) 
    {
        phy.x += phy.vx;
        phy.y += phy.vy;
    } 

    return true;
}
