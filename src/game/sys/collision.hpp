#pragma once

#include <ecs/util/typealiases.hpp>

//namespace GAME {

// Forward declaration
struct GameContext_t;

template<typename GameCTX_t>
struct CollisionSystem_t
{
    explicit CollisionSystem_t() = default;    

    bool Update(GameCTX_t& g) const;   

    private:
        
};

//}
