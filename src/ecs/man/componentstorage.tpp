#pragma once

#include "componentstorage.hpp"

namespace ECS {

    template<typename CMP_t>
    CMP_t& 
    componentStorage_t::CreateComponent(EntityID_t eid)
    {
        auto& v = GetComponents<CMP_t>();
        auto& cmp = v.emplace_back(eid);
        return cmp;
    }

    template<typename CMP_t>
    Vec_t<CMP_t>&
    componentStorage_t::CreateComponentVector()
    {
        auto typeID = CMP_t::getComponentTypeID();
        auto v = std::make_unique<ComponentVector_t<CMP_t>>();
        v->components.reserve(m_initialSize);
        auto* vecptr = v.get();
        //std::move(v) --> Cast
        m_ComponentVectors[typeID] = std::move(v);
        return vecptr->components;
    }

    template<typename CMP_t>
    Vec_t<CMP_t>&
    componentStorage_t::GetComponents()
    {
        // Return value optimization --> Having 2 returns if I use the same variable
        // the return will not create a copy
        Vec_t<CMP_t>* comvec {nullptr};

        auto typeID = CMP_t::getComponentTypeID();
        auto it = m_ComponentVectors.find(typeID);

        if (it != m_ComponentVectors.end())
        {
            // With Polimorfic classes always dynamic_cast
            // It can return null -> Test before to use it
            auto* v = dynamic_cast
            <ComponentVector_t<CMP_t>*>
            (
                it->second.get()
            );

             comvec = &v->components;
        }
        else
        {
            comvec = &CreateComponentVector<CMP_t>();
        }

        return *comvec;
    }

    template<typename CMP_t>
    const Vec_t<CMP_t>&
    componentStorage_t::GetComponents() const
    {
        // Return value optimization --> Having 2 returns if I use the same variable
        // the return will not create a copy
        Vec_t<CMP_t>* comvec {nullptr};

        auto typeID = CMP_t::getComponentTypeID();
        auto it = m_ComponentVectors.find(typeID);

        if (it != m_ComponentVectors.end())
        {
            // With Polimorfic classes always dynamic_cast
            // It can return null -> Test before to use it
            auto* v = dynamic_cast
            <ComponentVector_t<CMP_t>*>
            (
                it->second.get()
            );

             comvec = &v->components;
        }
        else
        {
            throw "No like this";
            //comvec = &CreateComponentVector();
        }

        return *comvec;
    }

    inline Component_t* componentStorage_t::DeleteComponentByTypeIDAndEntityID(ComponentTypeID_t cid, EntityID_t eid)
    {
        // First we look for the vector of the indicated type cid
        auto it = m_ComponentVectors.find(cid);
        if (it == m_ComponentVectors.end()) return nullptr;

        //  Hast_t<ComponentID_t, UPtr<ComponentVectorBase_t>> m_ComponentVectors; --> Second
        auto vecBase { it->second.get()};

        auto* cmpptr = vecBase->DeleteComponentByEntityId(eid);

        return cmpptr;
    }
}