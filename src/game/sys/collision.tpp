
//#include <util/gamecontext.hpp>

// Systems
#include "collision.hpp"

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/collider.hpp>

//namespace ECS {

    template<typename GameCTX_t>    
    CollisionSystem_t<GameCTX_t>::CollisionSystem_t(uint32_t w, uint32_t h)
        : m_w(w), m_h(h)
    {        
    }
   
    template<typename GameCTX_t>
    bool 
    CollisionSystem_t<GameCTX_t>::Update(GameCTX_t& g) const
    {
        for (auto& col : g.template GetComponents<ColliderComponent_t>()) 
        {            
            auto* phy = g.template GetRequiredComponent<PhysicsComponent_t>(col);

            if (!phy) { continue; }

            // Coordinate conversion -> to screen coordinates
            auto xl { phy->x + col.box.xLeft };
            auto xr { phy->x + col.box.xRight };
            auto yu { phy->y + col.box.yUp };
            auto yd { phy->y + col.box.yDown };

            if ( xl > m_w || xr > m_w)
            {
                phy->x -= phy->vx;
                phy->vx = -phy->vx;
            }

            if ( yu > m_h || yd > m_h)
            {
                phy->y -= phy->vy;
                phy->vy = -phy->vy;
            }
        }

        return true;
    }
//}