#include "ecs/util/keyboard.hpp"

namespace ECS {

    // Do with the const
    //bool keyboard_t::IsKeyPressed (KeySym k) const noexcept
    bool Keyboard_t::IsKeyPressed (KeySym k) noexcept
    {
        if (auto it = GetIterator(k))
            return (*it)->second;

        return false;
    };

    Keyboard_t::OptIter
    Keyboard_t::GetIterator(KeySym k) noexcept
    {
        // Get iterator for value
        // Optimizacion del valor de retorno
        auto it = m_pressedKeys.find(k);

        //Returns iterator
        if(it != m_pressedKeys.end())
            return it;
        
        // returns nothing
        return {};
    }
}