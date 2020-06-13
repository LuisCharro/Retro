#include <algorithm>
#include <iostream>

#include "ecs/man/entitymanager.hpp"

//#include <game/cmp/physics.hpp>
//#include <game/cmp/render.hpp>



namespace ECS
{
    EntityManager_t::EntityManager_t()
    {
        m_Entity.reserve(1000);
        //m_Entity.resize(10);
    }

    void 
    EntityManager_t::DestroyEntityByID(EntityID_t eid)
    {
        std::cout << "DestroyEntityByID: " << eid << std::endl;
        auto* entity {GetEntityByID(eid)};

        if (!entity) return;

        std::cout << "Start to process all the components of the selected entity " << std::endl;

        // for(auto it = (*entity).begin(); it != (*entity).end(); ++it)
        //for(auto& [typeID, cmp]: *entity)
        for(auto& [typeID, _]: *entity)
        {
            std::cout << "Component: " << typeID << "To Eliminate"<< std::endl;
            m_components.DeleteComponentByTypeIDAndEntityID(typeID, eid);
        }

        //Copy paste change
        auto it =
        std::find_if(m_Entity.begin(),m_Entity.end(),
            [&eid](const auto& e) {return e.getEntityID() == eid;}
        );
        
        m_Entity.erase(it);
    }

    //DNRY
    const Entity_t* 
    EntityManager_t::GetEntityByID(EntityID_t eid) const 
    {
        auto it =
        std::find_if(
            m_Entity.begin(),
            m_Entity.end(),
            //Predicate function that returns true or false
            //using lambda expressions
            [&eid](const auto& e) {return e.getEntityID() == eid;}
        );

        if (it == m_Entity.end())
            return nullptr;

        return it.base();
    }

    Entity_t* 
    EntityManager_t::GetEntityByID(EntityID_t eid) 
    {
        // First, I became const --> To be able to call the version const
        auto eptr = const_cast<const EntityManager_t*>(this)->GetEntityByID(eid);
        // Then, because the returned object is already const --> I remove the const
        return const_cast<Entity_t*>(eptr);
    }
}