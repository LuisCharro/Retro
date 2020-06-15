#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

#include <ecs/util/typealiases.hpp>

namespace ECS
{
    struct ComponentVectorBase_t {
    // I need to call to the destructors of the derivated classes
    virtual ~ComponentVectorBase_t() = default;

    virtual Component_t* DeleteComponentByEntityId(EntityID_t eid) = 0;
};

template<typename CMP_t>
struct ComponentVector_t : ComponentVectorBase_t
{
    Vec_t<CMP_t> components;

    constexpr auto FindComponentIteratorById(EntityID_t eid) noexcept
    {
        std::optional itopt =
            std::find_if(components.begin(), components.end(),
                [&eid](CMP_t& cmp) { return cmp.GetEntityID() == eid; }
            );

        if ( *itopt == components.end()) itopt.reset();

        return itopt;
    }
    

    Component_t* DeleteComponentByEntityId(EntityID_t eid) override
    {
        auto itopt = FindComponentIteratorById(eid);
        if (!itopt) return nullptr;

        auto it = *itopt;

        std::cout << "Removing component [EID: " << eid << " , CID: " << it->getComponentTypeID() << "]" << std::endl;

        if (it + 1 != components.end())
        {
            // Copy the last item of the vector            
            *it = components.back(); //it ->operator=(components.back());            
        }

        // Delete the last item of the vector
        components.pop_back();

        return it.base();
    }
};

}