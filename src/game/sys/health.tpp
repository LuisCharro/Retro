
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
        
    }
}
