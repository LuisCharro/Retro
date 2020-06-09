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
    
    bool Update(GameCTX_t& g) const;
    

    private:
        constexpr void CheckBoundaryCollisions(const ColliderComponent_t& c, PhysicsComponent_t&p) const noexcept;
        constexpr BoundingBox_t Move2ScreenCoords(const BoundingBox_t& c, uint32_t x, uint32_t y) const noexcept;
        constexpr void CheckObjectCollision(BoundingBoxNode_t& bn1, BoundingBoxNode_t& bn2, const PhysicsComponent_t& p1, const PhysicsComponent_t& p2) const noexcept;

        const uint32_t m_w { 0 }, m_h { 0 };
        
};

//}
