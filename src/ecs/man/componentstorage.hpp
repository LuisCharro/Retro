#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include <ecs/util/typealiases.hpp>

#include <ecs/man/componentvector.hpp>

namespace ECS
{
    struct componentStorage_t
    {
        explicit componentStorage_t(std::size_t init) : m_initialSize(init) { }

        componentStorage_t(const componentStorage_t& ) = delete;
        componentStorage_t(componentStorage_t&& )      = delete;
        componentStorage_t& operator= (const componentStorage_t& ) = delete;
        componentStorage_t& operator= (componentStorage_t&& )      = delete;        

        template<typename CMP_t>        CMP_t&          CreateComponent(EntityID_t eid);
        template<typename CMP_t>        Vec_t<CMP_t>&   CreateComponentVector();
        template<typename CMP_t>        Vec_t<CMP_t>&   GetComponents();
        template<typename CMP_t> const  Vec_t<CMP_t>&   GetComponents() const;

        Component_t* DeleteComponentByTypeIDAndEntityID(ComponentTypeID_t cid, EntityID_t eid);
    
        private:

            Hast_t<ComponentID_t, UPtr<ComponentVectorBase_t>> m_ComponentVectors;
            std::size_t m_initialSize { 100 };
    };
}