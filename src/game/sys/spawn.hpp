#pragma once

#include <ecs/util/typealiases.hpp>

//namespace GAME {

struct SpawnerComponent_t;
struct PhysicsComponent_t;


template<typename GameCTX_t>
struct SpawnSystem_t
{
    explicit SpawnSystem_t() = default;

    PhysicsComponent_t* GetRequiredComponent(GameCTX_t& g, SpawnerComponent_t& spw) const;
    
    void SpawnGhost(GameCTX_t& g, uint32_t x, uint32_t y) const;
    void Update(GameCTX_t& g) const;    
};

//}
