#pragma once

#include <cstdint>
#include <vector>
#include <string_view>

#include <ecs/cmp/entity.hpp>
#include <ecs/man/componentstorage.hpp>

#include <ecs/util/typealiases.hpp>

  // EM: 8 bytes uint32_t b setA()
  // GC: 4 bytes uint32_t a setA()
  
  // Em array [8] |4.4||4.4||4.4||4.4||4.4||4.4||4.4||4.4|
  // GC* p = &array[0]
  // p->a = 5

  // vtable (tabla de punteros para llamar a las funciones del hijo)
  // EM: 16 bytes uint32_t b virtual setA()
  // GC: *12 bytes uint32_t a virtual setA()

namespace ECS{

struct EntityManager_t
{
    static constexpr std::size_t kNUMINITIALENTITIES {1000};
    static constexpr std::size_t kNUMINITIALCOMPONENTS {1000};

    explicit EntityManager_t();

    Entity_t& CreateEntity()
    {
      return m_Entity.emplace_back();
    }

    void DestroyEntityByID(EntityID_t eid);

    template <typename CMP_t>
    Entity_t* GetEntityPointerFromComponent(const CMP_t& cmp)
    {
        return GetEntityByID(cmp.GetEntityID());        
    }

    template <typename CMP_t>
    const Entity_t* GetEntityPointerFromComponent(const CMP_t& cmp) const
    {
        return GetEntityByID(cmp.GetEntityID());        
    }

    template <typename ReqCMP_t, typename CMP_t>
    const ReqCMP_t* GetRequiredComponent(const CMP_t& cmp) const
    {
      auto* e = GetEntityPointerFromComponent(cmp);
      
      if (e) return e->template getComponent<ReqCMP_t>();
      return nullptr;
    }

    template <typename ReqCMP_t, typename CMP_t>
    ReqCMP_t* GetRequiredComponent(const CMP_t& cmp)
    {
      const ReqCMP_t* rc = const_cast<const EntityManager_t*>(this)->GetRequiredComponent<ReqCMP_t>(cmp);
      return const_cast<ReqCMP_t*>(rc);
    }

    const Entity_t* GetEntityByID(EntityID_t eid) const;
          Entity_t* GetEntityByID(EntityID_t eid)      ;

    const Vec_t<Entity_t>& GetEntities() const { return m_Entity; }
    Vec_t<Entity_t>& GetEntities() { return m_Entity; }


    template <typename CMP_t>
    CMP_t&
    AddComponent(ECS::Entity_t& e)
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
    GetComponents() const {
      return m_components.GetComponents<CMP_t>();
    };

    template<typename CMP_t>
    std::vector<CMP_t>& 
    GetComponents() {
      return m_components.GetComponents<CMP_t>();
    };

    private:
      Vec_t<Entity_t> m_Entity {};
      componentStorage_t m_components {kNUMINITIALCOMPONENTS};
};

}