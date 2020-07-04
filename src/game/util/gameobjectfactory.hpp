#pragma once

#include <cstdint>
#include <string_view>

#include <game/cmp/collider.hpp>
#include <game/cmp/input.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/spawner.hpp>
#include <game/cmp/health.hpp>
#include <game/cmp/camera.hpp>

#include <ecs/man/entitymanager.tpp>

//Forward declaration
namespace ECS 
{
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
    
    ECS::Entity_t& CreateEntity(uint32_t x, uint32_t y, const std::string_view  filename) const;
    ECS::Entity_t& CreateRectangleEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) const;
    ECS::Entity_t& CreateSpriteEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t sprite[]) const;    
    ECS::Entity_t& CreatePlayer(uint32_t x, uint32_t y) const;
    ECS::Entity_t& CreateGhost(uint32_t x, uint32_t y) const;
    ECS::Entity_t& CreateCamera(uint32_t x, uint32_t y, uint32_t w, uint32_t h, ECS::EntityID_t followEID) const;

    ECS::Entity_t& CreatePlatform(uint32_t x, uint32_t y) const;

    void CreateLevel1() const;
    void LoadLevelJSON(std::string_view filepath) const;
    void Json2Bin(std::string_view jsonpath, std::string_view binpath) const;
    void LoadLevelBin(std::string_view filepath) const;

    template <typename CALLABLE_t>
    ECS::Entity_t& 
    CreateSpawner(uint32_t x, uint32_t y, CALLABLE_t callback) const
    {
        auto& e  = m_EntityMan.CreateEntity();
        auto& spw = m_EntityMan.AddComponent<SpawnerComponent_t>(e);
        auto& phy = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
        auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
        auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);

        rn.transparency = true;
        rn.LoadFromFile("assets/tomb.png");

        //cl.mask = ColliderComponent_t::L_NoLayer;
        cl.mask = ColliderComponent_t::L_Boundaries;

        cl.boxRoot.box.xLeft = 0;
        cl.boxRoot.box.xRight = rn.w;
        cl.boxRoot.box.yUp = rn.h;
        cl.boxRoot.box.yDown = 0;    

        phy.x = x; phy.y = y;
        phy.vy = 1;
        phy.gravity = 0;

        // Adding a delegate by Lambda expresion
        // We need to pass x and y as value (not by reference) because:
        //  - When the SpawnMethod is called the variables x and y does not exist (scope of variables)
        spw.SpawnMethod = callback;

        spw.to_be_spawned = 5;
       
        return e;
    }

    private:
        ECS::EntityManager_t& m_EntityMan;
};