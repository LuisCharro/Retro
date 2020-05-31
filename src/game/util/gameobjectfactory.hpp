#pragma once

#include <cstdint>
#include <string_view>

//Forward declaration
namespace ECS {
    struct EntityManager_t;
    struct Entity_t;
}

constexpr uint32_t kR = 0x00FF0000;
    constexpr uint32_t kG = 0x0000FF00;
    constexpr uint32_t kB = 0x000000FF;
  
    constexpr uint32_t sprite[8*8] =
    {
        kG,kG,kG,kG,kG,kG,kG,kG,
        kG,kB,kR,kR,kR,kR,kB,kG,
        kG,kB,kR,kG,kG,kG,kB,kG,
        kG,kB,kR,kR,kG,kG,kB,kG,
        kG,kB,kB,kR,kR,kG,kB,kG,
        kG,kB,kB,kB,kB,kR,kB,kG,
        kG,kB,kR,kR,kR,kR,kB,kG,
        kG,kG,kG,kG,kG,kG,kG,kG,
    };

struct GameObjectFactory_t
{
    explicit GameObjectFactory_t (ECS::EntityManager_t& em)
    : m_EntityMan(em)
    {
    }
    
    ECS::Entity_t& CreateEntity(uint32_t x, uint32_t y, const std::string_view  filename);
    ECS::Entity_t& CreateRectangleEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color);
    ECS::Entity_t& CreateSpriteEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t sprite[]);    
    ECS::Entity_t& CreatePlayer(uint32_t x, uint32_t y);
    ECS::Entity_t& CreateEnemy(uint32_t x, uint32_t y);
    ECS::Entity_t& CreateSpawner(uint32_t x, uint32_t y);

    private:
        ECS::EntityManager_t& m_EntityMan;
};
