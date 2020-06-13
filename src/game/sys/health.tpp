
// Systems
#include "health.hpp"

#include <ecs/cmp/entity.hpp>

// Components
#include <game/cmp/health.hpp>

template<typename GameCTX_t>
constexpr void 
HealthSystem_t<GameCTX_t>::Update(GameCTX_t& g) const noexcept
{
    for (auto& h : g.template GetComponents<HealthComponent_t>()) 
    {
        const auto* c { g.template GetRequiredComponent<ColliderComponent_t>(h)};

        if (c == nullptr) continue;

        if (h.health && LeafNodeCollided(c->boxRoot))
        {
            if (--h.health == 0)
            {
                std::cout <<  "------->Entity: " << h.GetEntityID() << " is dead" << std::endl;
                g.DestroyEntityByID(h.GetEntityID());
            }
            else
            {
                std::cout <<  "------->Entity: " << h.GetEntityID() << " life: " << h.health << std::endl;
            }            
        }
    }
}

template<typename GameCTX_t>
constexpr bool
HealthSystem_t<GameCTX_t>::LeafNodeCollided(const BoundingBoxNode_t& bn) const noexcept
{
    if (bn.childs.empty()) 
    {
        return bn.collided;
    }
    else if (bn.collided)        
    {
        for (const auto & b: bn.childs)
        {
            if (LeafNodeCollided(b))
            {
                return true;
            }
        }
    }
    
    return false;
}
