#pragma once

#include <cstdint>
#include <vector>
#include <string_view>

#include <ecs/man/componentstorage.tpp>
#include <ecs/cmp/entity.hpp>

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

    // Entities

    Entity_t& CreateEntity();
    void DestroyEntityByID(EntityID_t eid);

    const Entity_t* GetEntityByID(EntityID_t eid) const;
          Entity_t* GetEntityByID(EntityID_t eid)      ;

    const Vec_t<Entity_t>& GetEntities() const { return m_Entity; }
          Vec_t<Entity_t>& GetEntities()       { return m_Entity; }    

    // Components (template)

    template <typename CMP_t> CMP_t& AddComponent(ECS::Entity_t& e);    

    template <typename CMP_t> const std::vector<CMP_t>& GetComponents() const;
    template <typename CMP_t>       std::vector<CMP_t>& GetComponents();

    template <typename ReqCMP_t, typename CMP_t>  const ReqCMP_t* GetRequiredComponent(const CMP_t& cmp) const;
    template <typename ReqCMP_t, typename CMP_t>        ReqCMP_t* GetRequiredComponent(const CMP_t& cmp);

    // Entities (template)

    template <typename CMP_t> Entity_t*       GetEntityPointerFromComponent(const CMP_t& cmp);
    template <typename CMP_t> const Entity_t* GetEntityPointerFromComponent(const CMP_t& cmp) const;

    private:
      Vec_t<Entity_t> m_Entity {};
      componentStorage_t m_components {kNUMINITIALCOMPONENTS};
};

}