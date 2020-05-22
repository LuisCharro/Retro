#pragma once

#include<cstdint>

#include <X11/X.h>
#include <X11/keysym.h>


#include <ecs/cmp/component.hpp>

//namespace GAME {

struct InputComponent_t : public ECS::ComponentBase_t<InputComponent_t>
{
    //The constructor is incomplete once we had added herency --> We can not use default
    //explicit PhysicsComponent_t () = default;

    explicit InputComponent_t (ECS::EntityID_t eid)
    : ComponentBase_t(eid)//Component_t(eid)
    {
    }
        KeySym Key_LEFT {XK_o};
        KeySym Key_RIGHT {XK_p};
        KeySym Key_UP {XK_q};
        KeySym Key_DOWN {XK_a};
};


//}