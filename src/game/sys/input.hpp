#pragma once

#include <cstdint>
#include <ecs/util/typealiases.hpp>
#include <ecs/util/keyboard.hpp>

//namespace GAME {

// Forward declaration
//struct GameContext_t; //<-- Commented to use templates intead of a base class

template<typename GameCTX_t>
struct InputSystem_t
{
    //explicit InputSystem_t() = default;    
    explicit InputSystem_t();    

    //bool Update(GameContext_t& g) const;   
    bool Update(GameCTX_t& g) const;   

    private:

        static void OnKeyPress(KeySym k);
        static void OnKeyReleased(KeySym k);

        inline static ECS::Keyboard_t m_Keyboard {};//= Keyboard_t();   
};

//}
