#include <X11/X.h>
#include <X11/keysym.h>

// Systems
#include "input.hpp"

// Components
#include <game/cmp/input.hpp>
#include <game/cmp/physics.hpp>

extern "C"
{
    #include "lib/tinyPTC/src/tinyptc.h"
}

//namespace GAME {

    template<typename GameCTX_t>
    InputSystem_t<GameCTX_t>::InputSystem_t()
    {
        ptc_set_on_keypress(OnKeyPress);
        ptc_set_on_keyrelease(OnKeyReleased);
    }

    template<typename GameCTX_t>
    void 
    InputSystem_t<GameCTX_t>::OnKeyPress(KeySym k){
        m_Keyboard.KeyPressed(k);
    }

    template<typename GameCTX_t>
    void 
    InputSystem_t<GameCTX_t>::OnKeyReleased(KeySym k){
        m_Keyboard.KeyReleased(k);
    }

    //A lamda function is convertible to PointerToFunction when it does not capture

    template<typename GameCTX_t>
    bool 
    InputSystem_t<GameCTX_t>::Update(GameCTX_t& g) const
    {
        ptc_process_events();
        
        for (auto& inp : g.template GetComponents<InputComponent_t>()) 
        {
            auto* e = g.GetEntityByID(inp.GetEntityID());

            if (e)
            {   
                auto* phy = e->template getComponent<PhysicsComponent_t>();

                if (phy)
                {
                    phy->vx = phy->vy =  0;                

                    if (m_Keyboard.IsKeyPressed(inp.Key_LEFT))  { phy->vx = -3;}
                    if (m_Keyboard.IsKeyPressed(inp.Key_RIGHT)) { phy->vx = 3; }
                    if (m_Keyboard.IsKeyPressed(inp.Key_UP))    { phy->vy = -3;}
                    if (m_Keyboard.IsKeyPressed(inp.Key_DOWN))  { phy->vy = 3; }
                }
            }
        }

        return true;
    }
//}