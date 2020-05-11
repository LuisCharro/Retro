#pragma once

#include <cstdint>

#include <ecs/cmp/component.hpp>

//namespace GAME {

struct PhysicsComponent_t : public ECS::Component_t
{
    //The constructor is incomplete once we had added herency
    //explicit PhysicsComponent_t () = default;

    explicit PhysicsComponent_t (ECS::EntityID_t eid)
    : ECS::Component_t(eid)
    {
    }

    static ECS::ComponentTypeID_t getComponentTypeID() noexcept {
        return static_cast<ECS::ComponentTypeID_t>(2);
    }

    //private:
        uint32_t x {0}, y {0};
        int32_t vx {1}, vy {1}; 
    
};

//}