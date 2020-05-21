#pragma once

#include <ecs/util/typealiases.hpp>

//namespace GAME {

// Forward declaration
//struct GameContext_t;
struct ColliderComponent_t;

template<typename GameCTX_t>
struct CollisionSystem_t
{
    explicit CollisionSystem_t(uint32_t w, uint32_t h);

    PhysicsComponent_t* GetRequiredComponent(GameCTX_t& g, ColliderComponent_t& col) const;
    
    bool Update(GameCTX_t& g) const;   

    private:
        const uint32_t m_w { 0 }, m_h { 0 };
        
};

//}
