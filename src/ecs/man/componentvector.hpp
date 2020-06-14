#pragma once

#include <iostream>
#include <vector>
#include <algorithm>

#include <ecs/util/typealiases.hpp>

namespace ECS
{
    struct ComponentVectorBase_t {
    // I need to call to the destructors of the derivated classes
    virtual ~ComponentVectorBase_t() = default;

    virtual void DeleteComponentByEntityId(EntityID_t eid) = 0;
};

template<typename CMP_t>
struct ComponentVector_t : ComponentVectorBase_t
{
    Vec_t<CMP_t> components;

    virtual void DeleteComponentByEntityId(EntityID_t eid) override final
    {
        //TODO: Linear search --> I need to change it
        auto it =
            std::find_if(components.begin(), components.end(),
                [&eid](CMP_t& cmp) { return cmp.GetEntityID() == eid; }
            );

        if (it == components.end()) return;

        std::cout << "Removing component [EID: " << eid << " , CID: " << it->getComponentTypeID() << "]" << std::endl;

        components.erase(it);
    }
};

}