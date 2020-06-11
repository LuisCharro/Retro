#pragma once

#include <cstdint>
#include <ecs/util/typealiases.hpp>

template<typename GameCTX_t>
struct PhysicsSystem_t
{
    explicit PhysicsSystem_t() = default;    

    bool Update(GameCTX_t& g) const;        
};
