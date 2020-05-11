
//#include <util/gamecontext.hpp>

// Systems
#include "collision.hpp"

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>

//namespace ECS {

    template<typename GameCTX_t>
    bool 
    CollisionSystem_t<GameCTX_t>::Update(GameCTX_t& g) const
    {
        for (auto& phy : g.template GetComponents<PhysicsComponent_t>()) 
        {
            auto* e = g.GetEntityByID(phy.GetEntityID());

            if (e)
            {
                auto* ren = e->template getComponent<RenderComponent_t>();

                if (ren)
                {
                    if (phy.x > 640|| phy.x + ren->w > 640)
                    {
                        phy.x -= phy.vx;
                        phy.vx = -phy.vx;
                    }

                    if (phy.y > 360|| phy.y + ren->h > 360)
                    {
                        phy.y -= phy.vy;
                        phy.vy = -phy.vy;
                    }
                }
            }  
        }

        return true;
    }
//}