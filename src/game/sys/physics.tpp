#pragma once

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
        // Verify if we are still on platform       
        if (std::abs(phy.vy) > phy.KMinVyForNotOnPlatform)
            phy.onPlatform = false;

        // Jump
        if (phy.jumpIdx < phy.jumpTable.size())
        {
            phy.vy = phy.jumpTable[phy.jumpIdx];
            ++phy.jumpIdx;
        }

        // Gravity
        phy.vy += phy.gravity;        
        std::clamp(phy.vy, phy.KMinVy, phy.KMaxVy);

        // Acceleration
        phy.vx += phy.ax;        
        std::clamp(phy.vx, phy.KMinVx, phy.KMaxVx);

        // Update positions
        phy.x += phy.vx;
        phy.y += phy.vy;
    } 

    return true;
}
