#pragma once

#include <cstdint>

#include <ecs/cmp/component.hpp>
#include "box.hpp"

struct ColliderComponent_t : public ECS::ComponentBase_t<ColliderComponent_t>
{
    //The constructor is incomplete once we had added herency
    //explicit PhysicsComponent_t () = default;

    explicit ColliderComponent_t (ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {
    }

    // Composite pattern
    BoundingBoxNode_t boxRoot {};
};