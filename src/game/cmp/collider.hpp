#pragma once

#include <cstdint>

#include <ecs/cmp/component.hpp>

//namespace GAME {

struct BoundingBox_t
{
    uint32_t xLeft {0}, xRight {0};
    uint32_t yUp {0}, yDown {0}; 
};


struct ColliderComponent_t : public ECS::ComponentBase_t<ColliderComponent_t>
{
    //The constructor is incomplete once we had added herency
    //explicit PhysicsComponent_t () = default;

    explicit ColliderComponent_t (ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {
    }

    static ECS::ComponentTypeID_t getComponentTypeID() noexcept {
        return static_cast<ECS::ComponentTypeID_t>(4);
    }

    BoundingBox_t box;
};

//}