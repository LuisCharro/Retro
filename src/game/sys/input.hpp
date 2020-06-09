#pragma once

#include <cstdint>
#include <ecs/util/typealiases.hpp>
#include <ecs/util/keyboard.hpp>

template<typename GameCTX_t>
struct InputSystem_t
{
    explicit InputSystem_t();    

    bool Update(GameCTX_t& g) const;

    static bool IsKeyPressed(KeySym k);

    private:

        static void OnKeyPress(KeySym k);
        static void OnKeyReleased(KeySym k);       

        inline static ECS::Keyboard_t m_Keyboard {};
};
