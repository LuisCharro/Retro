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

        protected:
            // Declaration + Definition (inline) [No need to define in the ".cpp"]
            inline static ComponentTypeID_t nextTypeId {0};

        private:

            EntityID_t entityID {0};
    };

    // Added a Base class to use CRTP (Curiously Recurring Template Pattern) 
    // static inheritance

    template<typename TYPE>
    struct ComponentBase_t : Component_t
    {
        explicit ComponentBase_t(EntityID_t eid) 
            : Component_t(eid) {}

        //For every type used --> I will get "an Id" 
        //This will be used for the class componentStorage_t to select Vec_t<CMP_t> components with that Id
        static ComponentTypeID_t getComponentTypeID() noexcept
        {
            static ComponentTypeID_t typeID {++nextTypeId};
            return typeID;
        }
    };    
}