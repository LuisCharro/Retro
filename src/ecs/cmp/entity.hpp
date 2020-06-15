#pragma once

//Lots of includes may require a change

#include <cstdint>

#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/input.hpp>

#include <ecs/util/typealiases.hpp>

namespace ECS {

    struct Entity_t
    {
        
        // Indication to use the default constructor --> = default
        explicit Entity_t() = default;

        template<typename CMP_t>
        void addComponent(CMP_t& cmp)
        {
            auto type = cmp.getComponentTypeID();
            m_components[type] = &cmp;
        }

        template<typename CMP_t>    
        CMP_t* getComponent()
        {
            auto type = CMP_t::getComponentTypeID();
            auto it = m_components.find(type);

            if (it != m_components.end())
            {
                return static_cast<CMP_t*>(it->second);
            }
            return nullptr;
        }

        template<typename CMP_t>
        const CMP_t* getComponent() const
        {
            auto* cmp = const_cast<Entity_t*> (this)->getComponent<CMP_t>();
            return const_cast<const CMP_t*>(cmp);
        }

        constexpr EntityID_t getEntityID () const noexcept {return entiy_ID;}

        auto begin() { return m_components.begin(); }
        auto end()   { return m_components.end();   }

        void UpdateComponent(ComponentTypeID_t cid, Component_t* cmpptr)
        {
            auto it = m_components.find(cid);

            if (it != m_components.end())
            {
                it->second = cmpptr;
            }
        }

        private:

        //Optional --> Nothing or an Object
        //reference_Wrapper --> Object that contains a referece
        //std::optional<std::reference_wrapper<PhysicsComponent_t>> phy;

        // Initialy, we have the 3 pointers to the 3 components
        // Now we use the m_components

        // PhysicsComponent_t* phy {nullptr};
        // RenderComponent_t* ren {nullptr};
        // InputComponent_t* inp {nullptr};

        //unordered_map<...>
        Hast_t<ComponentTypeID_t, Component_t*> m_components;

        //std::size_t entiy_ID;
        EntityID_t entiy_ID {++nextID};

        // Declaration + Definition (inline)
        inline static EntityID_t nextID{0};
    };

//Definition --> Outside of the .hpp or I will create several times
// It goes to the cpp
//EntityID_t Entity_t::nextID = 0;

}



