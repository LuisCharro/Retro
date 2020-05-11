#pragma once

#include<cstdint>
#include <ecs/util/typealiases.hpp>

namespace ECS
{
    struct Component_t
    {
        explicit Component_t(EntityID_t eid)
        : entityID {eid}
        {

        }

        // If all the data is constant the compiler can do the calculation
        // and it saves the result instead put the code + calls
        constexpr EntityID_t GetEntityID() const noexcept {return entityID;}

        private:

            //Create the type ComponentID like in Entity
            ComponentID_t ComponentID {++nextID};
            EntityID_t entityID {0};

            // Declaration + Definition (inline)
            inline static ComponentID_t nextID {0};
    };
    
}