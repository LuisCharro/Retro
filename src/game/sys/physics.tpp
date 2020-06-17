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
        // Count times Vy is 0        
        if (phy.vy == 0)            
        {
            constexpr uint8_t z { 0 };
            phy.timesVyIs0 = std::clamp(phy.timesVyIs0, z, phy.KTimesVyIs0ToJump);            
        }
        else
        {
            phy.timesVyIs0 = 0;
        }        

        // Jump
        if (phy.jumpIdx < phy.jumpTable.size())
        {
            phy.vy = phy.jumpTable[phy.jumpIdx];
            ++phy.jumpIdx;
        }

        // Gravity
        phy.vy += phy.gravity;        
        std::clamp(phy.vy, phy.KMinVy, phy.KMaxVy);

        // Update positions
        phy.x += phy.vx;
        phy.y += phy.vy;
    } 

    return true;
}
