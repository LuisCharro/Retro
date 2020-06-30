#pragma once

// Systems
#include "collision.hpp"

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/collider.hpp>
#include <game/cmp/health.hpp>

#include <functional>
//#include <bitset>

template<typename GameCTX_t>    
CollisionSystem_t<GameCTX_t>::CollisionSystem_t(uint32_t w, uint32_t h)
    : m_w(w), m_h(h)
{        
}

template<typename GameCTX_t>
bool 
CollisionSystem_t<GameCTX_t>::Update(GameCTX_t& g) const
{
    auto& ColCmpVec {g.template GetComponents<ColliderComponent_t>()};

    //Clean previous collisions (Using a type)
    std::function<void(BoundingBoxNode_t&)> falseCollideds = 
        [&](BoundingBoxNode_t& b)
        {
            b.collided = false;
            for (auto& c: b.childs) falseCollideds(c);
        };

    for (auto& c: ColCmpVec) falseCollideds(c.boxRoot);

    for (std::size_t i=0; i < ColCmpVec.size(); ++i) 
    {
        auto& c1 {ColCmpVec[i]};
        auto* p1 = g.template GetRequiredComponent<PhysicsComponent_t>(c1);

        if (!p1) { continue; }

        if (c1.mask & ColliderComponent_t::L_Boundaries)
            CheckBoundaryCollisions(c1, *p1);

        for (std::size_t j=i+1; j < ColCmpVec.size(); ++j) 
        {
            auto& c2 {ColCmpVec[j]};

            if ( (c1.mask & c2.mask) == 0) { continue; }

            auto* p2 = g.template GetRequiredComponent<PhysicsComponent_t>(c2);

            if (!p2) { continue; }

            if (CheckObjectCollision(c1.boxRoot, c2.boxRoot, *p1, *p2))
            {
                ReactToCollision(g, c1, c2);
            }
        }    
    }

    return true;
}

template<typename GameCTX_t>
constexpr bool  
CollisionSystem_t<GameCTX_t>::CheckObjectCollision(
    BoundingBoxNode_t& bn1, 
    BoundingBoxNode_t& bn2, 
    const PhysicsComponent_t& p1, 
    const PhysicsComponent_t& p2) const noexcept
{
    // Move Boundng Boxes to screen coordinates
    auto b1 = Move2WorldCoords(bn1.box, p1.x, p1.y);
    auto b2 = Move2WorldCoords(bn2.box, p2.x, p2.y);

    // Check collisions in one generic axis
    auto checkIntervals = [](float L1,float R1,float L2,float R2)
    {
        // 2 Possibilities 
        // I1   L1--------R1               |              L1--------R1
        // I2                 L2--------R2 | L2--------R2
        if (L2 > R1) return false;
        if (L1 > R2) return false;
        return true;
    };

    // Check collision in both axes
    if  (
            checkIntervals(b1.xLeft, b1.xRight, b2.xLeft, b2.xRight)
            &&
            checkIntervals(b1.yUp  , b1.yDown , b2.yUp  , b2.yDown )
        )
    {
        // 4 cases
        // 2 Leaft node
        // BN1 has child nodes
        // BN2 has child nodes
        // BN1 and BN2 have child nodes

        //Collision
        bn1.collided = true;
        bn2.collided = true;       

        if (!bn1.childs.empty() )
        {
            for(auto& b: bn1.childs)
            {
                if (CheckObjectCollision(b, bn2, p1, p2))
                {
                    // std::cout << "Collision" << std::endl;
                    // std::cout << "b1.xLeft: " << b1.xLeft << " b1.xRight: " << b1.xRight << std::endl; 
                    // std::cout << "b2.xLeft: " << b2.xLeft << " b2.xRight: " << b2.xRight << std::endl; 
                    // std::cout << "b1.yUp  : " << b1.yUp << " b1.yDown: " << b1.yDown << std::endl; 
                    // std::cout << "b2.yUp  : " << b2.yUp << " b2.yDown: " << b2.yDown << std::endl; 
                    return true;                   
                }
            } 
        }
        else if (!bn2.childs.empty() )
        {
            for(auto& b: bn2.childs)
            {
                if (CheckObjectCollision(bn1, b, p1, p2))
                {
                    // std::cout << "Collision" << std::endl; 
                    // std::cout << "b1.xLeft: " << b1.xLeft << " b1.xRight: " << b1.xRight << std::endl; 
                    // std::cout << "b2.xLeft: " << b2.xLeft << " b2.xRight: " << b2.xRight << std::endl; 
                    // std::cout << "b1.yUp  : " << b1.yUp << " b1.yDown: " << b1.yDown << std::endl; 
                    // std::cout << "b2.yUp  : " << b2.yUp << " b2.yDown: " << b2.yDown << std::endl; 
                    return true;                    
                }
            } 
        }
        else // Collision of 2 leaft colliders
        {
            // std::cout << "Collision" << std::endl; 
            // std::cout << "b1.xLeft: " << b1.xLeft << " b1.xRight: " << b1.xRight << std::endl; 
            // std::cout << "b2.xLeft: " << b2.xLeft << " b2.xRight: " << b2.xRight << std::endl; 
            // std::cout << "b1.yUp  : " << b1.yUp << " b1.yDown: " << b1.yDown << std::endl; 
            // std::cout << "b2.yUp  : " << b2.yUp << " b2.yDown: " << b2.yDown << std::endl; 
            return true;
        }    
    }
    return false; 
}

template<typename GameCTX_t>
constexpr void  
CollisionSystem_t<GameCTX_t>::InflictDamage(GameCTX_t& g, ColliderComponent_t& inflicted, ColliderComponent_t& receiver) const noexcept
{
    std::cout << "ReactToCollision with P_Damages " << std::endl; 

    auto* recevHealth = g.template GetRequiredComponent<HealthComponent_t>(receiver);
    auto* inflHealth = g.template GetRequiredComponent<HealthComponent_t>(inflicted);

    if (!recevHealth || !inflHealth) { return; }

    recevHealth->damage += inflHealth->damageInflicted;
    inflHealth->damage += inflHealth->selfDamageOnInfliction; 
}

template<typename GameCTX_t>
constexpr void  
CollisionSystem_t<GameCTX_t>::UndoCollision(GameCTX_t& g, ColliderComponent_t& solid, ColliderComponent_t& mobile) const noexcept
{
    auto* phySolid = g.template GetRequiredComponent<PhysicsComponent_t>(solid);
    auto* phymobile = g.template GetRequiredComponent<PhysicsComponent_t>(mobile);

    if (!phySolid || !phymobile) return;

    auto solidBox = Move2WorldCoords(solid.boxRoot.box, phySolid->x, phySolid->y);
    auto mobileBox = Move2WorldCoords(mobile.boxRoot.box, phymobile->x, phymobile->y);

    //           |----|     Right
    // |----|               Left
    // |--------------|     Center
    //      |----|          Center
    //   |----------|

    // Calculate intersection
    // Trailing Return Type
    auto intervalIntersection =[](float Ml, float Mr, float Sl, float Sr) -> float
    {
        // Left
        if (Ml < Sl)
        {
            if (Mr < Sr)   { return Sl - Mr; }        
        }
        // Right
        else if (Mr > Sr)  { return Sr - Ml; }
                             return 0;
    };

    struct { float x, y; } overlap {
            intervalIntersection(mobileBox.xLeft, mobileBox.xRight, solidBox.xLeft, solidBox.xRight)
        ,   intervalIntersection(mobileBox.yUp,   mobileBox.yDown,  solidBox.yUp,   solidBox.yDown)
    };

    // Stopping the collision
    if (overlap.x == 0 || (overlap.y != 0 && std::abs(overlap.y) <= std::abs(overlap.x) ))
    {
        phymobile->y += overlap.y;
        phymobile->vy = 0;

        //if (overlap.y < 0) phymobile->onPlatform = true;
        phymobile->onPlatform = (overlap.y < 0); // Its better do this calculation than use an If
        phymobile->vx *= (phymobile->friction * phySolid->friction);
    }
    else
    {
        phymobile->x += overlap.x;
        phymobile->vx = 0;
    }
}

template<typename GameCTX_t>
constexpr void  
CollisionSystem_t<GameCTX_t>::ReactToCollision(GameCTX_t& g, ColliderComponent_t& c1, ColliderComponent_t& c2) const noexcept
{
    using CP = ColliderComponent_t;
    CP *player {&c1}, *other {&c2};

    if (c2.properties & CP::P_IsPlayer )
    {
        std::swap(player,other);
    }
    else if ( !(c1.properties & CP::P_IsPlayer) )  // Now only interesting in collisions to the player
    {
        //std::cout << "ReactToCollision No player in collision " << std::endl; 
        return;
    }

    // std::bitset<8> x(other->properties);    
    // std::cout << "other->properties: " << x << std::endl; 

    if (other->properties & CP::P_Damages)
    {
        InflictDamage(g,*other,*player);
    }
    else if (other->properties & CP::P_IsSolid )
    {
        UndoCollision(g,*other,*player);
    }
}

template<typename GameCTX_t>
constexpr BoundingBox_t<float>  
CollisionSystem_t<GameCTX_t>::Move2WorldCoords(const BoundingBox_t<uint32_t>& box, float x, float y) const noexcept
{
    BoundingBox_t<float> screenBox 
    {
         x + box.xLeft 
        ,x + box.xRight
        ,y + box.yUp   
        ,y + box.yDown 
    };

    return screenBox;
}

template<typename GameCTX_t>
constexpr void  
CollisionSystem_t<GameCTX_t>::CheckBoundaryCollisions(const ColliderComponent_t& c, PhysicsComponent_t&p) const noexcept
{
    // Coordinate conversion --> to world coordinates
    auto b { Move2WorldCoords(c.boxRoot.box, p.x, p.y) };

    // Collisions
    if ( b.xLeft > m_w || b.xRight > 0) { p.x -= p.vx; p.vx = -p.vx; }
    if ( b.yUp   > m_h || b.yDown  > 0) 
    {
        p.y -= p.vy;

        if (p.gravity)
        {
            p.vy = 0; 
        }
        else
        {
            p.vy = -p.vy; 
        }        
    }
}
