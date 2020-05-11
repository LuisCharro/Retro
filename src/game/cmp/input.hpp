#pragma once

#include<cstdint>

#include <X11/X.h>
#include <X11/keysym.h>


#include <ecs/cmp/component.hpp>

//namespace GAME {

struct InputComponent_t : public ECS::Component_t
{
    //The constructor is incomplete once we had added herency
    //explicit PhysicsComponent_t () = default;

    explicit InputComponent_t (ECS::EntityID_t eid)
    : ECS::Component_t(eid)
    {
    }

    static ECS::ComponentTypeID_t getComponentTypeID() noexcept {
        return static_cast<ECS::ComponentTypeID_t>(1);
    }

    //private:
       KeySym Key_LEFT {XK_o};
       KeySym Key_RIGHT {XK_p};
       KeySym Key_UP {XK_q};
       KeySym Key_DOWN {XK_a};
};


//}