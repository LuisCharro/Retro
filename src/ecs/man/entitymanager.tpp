#pragma once

#include <algorithm>
#include <iostream>

#include "entitymanager.hpp"

namespace ECS
{
    
    inline EntityManager_t::EntityManager_t()
    {
        m_Entity.reserve(1000);        
    }

    //---------------------------------------------------------------------------
    // Entities
    //---------------------------------------------------------------------------

    inline Entity_t& EntityManager_t::CreateEntity() 
    { 
      return m_Entity.emplace_back(); 
    }

    inline void 
    EntityManager_t::DestroyEntityByID(EntityID_t eid)
    {
        std::cout << "DestroyEntityByID: " << eid << std::endl;
        auto* entity { GetEntityByID(eid) };

        if (!entity) return;

        std::cout << "Start to process all the components of the selected entity " << std::endl;

        // For each component inside an Entity
        // for(auto it = (*entity).begin(); it != (*entity).end(); ++it)
        // for(auto& [typeID, cmp]: *entity)
        for(auto& [typeID, _]: *entity)
        {
            std::cout << "Component: " << typeID << "To Eliminate"<< std::endl;
            auto* cmpptr = m_components.DeleteComponentByTypeIDAndEntityID(typeID, eid);

            if (!cmpptr) continue;

            auto* moveEntity { GetEntityByID( cmpptr->GetEntityID() ) };
            moveEntity->UpdateComponent(typeID, cmpptr);
        }

        //Copy paste from other method --> Change
        auto it =
        std::find_if(m_Entity.begin(),m_Entity.end(),
            [&eid](const auto& e) {return e.getEntityID() == eid;}
        );

        // By the moment --> I am deleting an Entity anywhere in the vector
        // Then, the vector will copy 1 by 1 each component until the end
        // The content of the entites are copiable by default copy
        // In the future --> I can do the same as the components
        // Copy the last of the vector to a temp
        // Move the entity that I want to delete to the end
        // Put the temp entity where it was the one that I want to delete

        m_Entity.erase(it);
    }

    inline Entity_t* 
    EntityManager_t::GetEntityByID(EntityID_t eid) 
    {
        // First, I became const --> To be able to call the version const
        auto eptr = const_cast<const EntityManager_t*>(this)->GetEntityByID(eid);
        // Then, because the returned object is already const --> I remove the const
        return const_cast<Entity_t*>(eptr);
    }

    //DNRY
    inline const Entity_t* 
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

    //---------------------------------------------------------------------------
    // Components (template)
    //---------------------------------------------------------------------------

    template <typename CMP_t>
    CMP_t&
    EntityManager_t::AddComponent(ECS::Entity_t& e)
    {
      CMP_t* cmp_ptr {e.getComponent<CMP_t>()};

      if (!cmp_ptr)
      {
        auto& cmp = m_components.CreateComponent<CMP_t>(e.getEntityID());
        e.addComponent(cmp);
        cmp_ptr = &cmp;
      }
      return *cmp_ptr;
    }

    template<typename CMP_t>
    const std::vector<CMP_t>& 
    EntityManager_t::GetComponents() const {
      return m_components.GetComponents<CMP_t>();
    };

    template<typename CMP_t>
    std::vector<CMP_t>& 
    EntityManager_t::GetComponents() {
      return m_components.GetComponents<CMP_t>();
    };

    template <typename ReqCMP_t, typename CMP_t>
    const ReqCMP_t* EntityManager_t::GetRequiredComponent(const CMP_t& cmp) const
    {
      auto* e = GetEntityPointerFromComponent(cmp);
      
      if (e) return e->template getComponent<ReqCMP_t>();
      return nullptr;
    }

    template <typename ReqCMP_t, typename CMP_t>
    ReqCMP_t* EntityManager_t::GetRequiredComponent(const CMP_t& cmp)
    {
      const ReqCMP_t* rc = const_cast<const EntityManager_t*>(this)->GetRequiredComponent<ReqCMP_t>(cmp);
      return const_cast<ReqCMP_t*>(rc);
    }

    //---------------------------------------------------------------------------
    // Entities (template)
    //---------------------------------------------------------------------------

    template <typename CMP_t>
    Entity_t* EntityManager_t::GetEntityPointerFromComponent(const CMP_t& cmp)
    {
        return GetEntityByID(cmp.GetEntityID());        
    }

    template <typename CMP_t>
    const Entity_t* EntityManager_t::GetEntityPointerFromComponent(const CMP_t& cmp) const
    {
        return GetEntityByID(cmp.GetEntityID());        
    }
}