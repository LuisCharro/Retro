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

    enum {
            L_NoLayer   = 0x00
        ,   L_Blades    = 0x01
        ,   L_Platforms = 0x02
        ,   L_All       = 0xFF
    };

    //
    uint8_t mask { L_All };

    // Composite pattern
    BoundingBoxNode_t boxRoot {};
};