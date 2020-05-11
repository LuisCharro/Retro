#pragma once

#include <vector>

#include <ecs/util/typealiases.hpp>

namespace ECS
{

struct ComponentVectorBase_t {
    // I need to call to the destructors of the derivated classes
    virtual ~ComponentVectorBase_t() = default;
};

template<typename CMP_t>
struct ComponentVector_t : ComponentVectorBase_t
{
    Vec_t<CMP_t> components;
};

struct componentStorage_t
{
    //componentStorage_t()=delete;

    explicit componentStorage_t(std::size_t init) 
        :   m_initialSize(init)
    {
        
    }

    componentStorage_t(const componentStorage_t& ) = delete;
    componentStorage_t(componentStorage_t&& ) = delete;

    componentStorage_t& operator= (const componentStorage_t& ) = delete;
    componentStorage_t& operator= (componentStorage_t&& ) = delete;

    template<typename CMP_t>
    CMP_t& 
    CreateComponent(EntityID_t eid)
    {
        auto& v = GetComponents<CMP_t>();
        auto& cmp = v.emplace_back(eid);
        return cmp;
    }

    //Example of the previous call without to use templates
    //Like this we can see the code result for 1 type

    template<typename CMP_t>
    Vec_t<CMP_t>& 
    CreateComponentVector()
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
    GetComponents()
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
    GetComponents() const 
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

    private:

        Hast_t<ComponentID_t, UPtr<ComponentVectorBase_t>> m_ComponentVectors;
        std::size_t m_initialSize { 100 };
};


}