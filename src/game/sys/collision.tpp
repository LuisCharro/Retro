// Systems
#include "collision.hpp"

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/collider.hpp>

#include <functional>

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

    //Using a type
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

        CheckBoundaryCollisions(c1, *p1);

        for (std::size_t j=i+1; j < ColCmpVec.size(); ++j) 
        {
            auto& c2 {ColCmpVec[j]};
            auto* p2 = g.template GetRequiredComponent<PhysicsComponent_t>(c2);

            if (!p2) { continue; }

            CheckObjectCollision(c1.boxRoot, c2.boxRoot, *p1, *p2);
        }    
    }

    return true;
}

template<typename GameCTX_t>
constexpr void  
CollisionSystem_t<GameCTX_t>::CheckObjectCollision(
    BoundingBoxNode_t& bn1, 
    BoundingBoxNode_t& bn2, 
    const PhysicsComponent_t& p1, 
    const PhysicsComponent_t& p2) const noexcept
{
    // Move Boundng Boxes to screen coordinates
    auto b1 = Move2ScreenCoords(bn1.box, p1.x, p1.y);
    auto b2 = Move2ScreenCoords(bn1.box, p2.x, p2.y);

    // Check collisions in one generic axis
    auto checkIntervals = [](uint32_t L1,uint32_t R1,uint32_t L2,uint32_t R2)
    {
        // I1   L1--------R1                            L1--------R1
        // I2                 L2--------R2 L2--------R2
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

        if (!bn1.childs.empty() )
        {
            for(auto& b: bn1.childs)
            {
                CheckObjectCollision(b, bn2, p1, p2);
            } 
        }
        else if (!bn2.childs.empty() )
        {
            for(auto& b: bn2.childs)
            {
                CheckObjectCollision(bn1, b, p1, p2);
            } 
        }
        else
        {
            //Collision
            bn1.collided = true;
            bn2.collided = true;

            std::cout << "Collision" << std::endl; 

            std::cout << "b1.xLeft: " << b1.xLeft << " b1.xRight: " << b1.xRight << std::endl; 
            std::cout << "b2.xLeft: " << b2.xLeft << " b2.xRight: " << b2.xRight << std::endl; 

            std::cout << "b1.yUp  : " << b1.yUp << " b1.yDown: " << b1.yDown << std::endl; 
            std::cout << "b2.yUp  : " << b2.yUp << " b2.yDown: " << b2.yDown << std::endl; 
        }
    }
}

template<typename GameCTX_t>
constexpr BoundingBox_t  
CollisionSystem_t<GameCTX_t>::Move2ScreenCoords(const BoundingBox_t& box, uint32_t x, uint32_t y) const noexcept
{
    BoundingBox_t screenBox 
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
    // Coordinate conversion --> to screen coordinates
    auto b { Move2ScreenCoords(c.boxRoot.box, p.x, p.y) };

    // Collisions
    if ( b.xLeft > m_w || b.xRight > m_w) { p.x -= p.vx; p.vx = -p.vx; }
    if ( b.yUp   > m_h || b.yDown  > m_h) { p.y -= p.vy; p.vy = -p.vy; }
}
