#include "gameobjectfactory.hpp"

#include <game/cmp/collider.hpp>
#include <game/cmp/input.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/spawner.hpp>

#include <ecs/cmp/entity.hpp>
#include "ecs/man/entitymanager.hpp"

ECS::Entity_t& 
GameObjectFactory_t::CreateRectangleEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);
    
    ph.x = x; ph.y = y;
    ph.vx = ph.vy = 3; 

    rn.SetDimensions(w,h);

    std::fill(begin(rn.sprite), end(rn.sprite),color);

    cl.box.xLeft = 0;
    cl.box.xRight = rn.w;
    cl.box.yUp = rn.h;
    cl.box.yDown = 0;

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateSpriteEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t sprite[])
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);

    ph.x = x; ph.y = y;
    ph.vx = ph.vy = 3; 

    rn.SetDimensions(w,h);

    unsigned dataArraySize = w*h;//sizeof(sprite) / sizeof(int);
        
    rn.sprite.resize(dataArraySize);

    std::copy ( sprite, sprite+dataArraySize , rn.sprite.begin());
    //https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping

    cl.box.xLeft = 0;
    cl.box.xRight = rn.w;
    cl.box.yUp = rn.h;
    cl.box.yDown = 0;

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateEntity(uint32_t x, uint32_t y, const std::string_view  filename)
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);

    rn.transparency = true;

    rn.LoadFromFile(filename);

    ph.x = x; ph.y = y;
    ph.vx = ph.vy = 3; 

    cl.box.xLeft = 0;
    cl.box.xRight = rn.w;
    cl.box.yUp = rn.h;
    cl.box.yDown = 0;

    return e;
}

ECS::Entity_t&  
GameObjectFactory_t::CreatePlayer(uint32_t x, uint32_t y)
{
    // Without the &, I will create a copy and I will not touch the variable    
    auto& e  = CreateEntity(x, y, "assets/ninja.png");
    m_EntityMan.AddComponent<InputComponent_t>(e);

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateEnemy(uint32_t x, uint32_t y)
{
    auto& e  = CreateEntity(x, y, "assets/fantasma.png");
    return e; 
}

ECS::Entity_t& 
GameObjectFactory_t::CreateSpawner(uint32_t x, uint32_t y)
{
    auto& e  = m_EntityMan.CreateEntity();
    [[maybe_unused]]auto& spw = m_EntityMan.AddComponent<SpawnerComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);

    ph.x = x; ph.y = y;

    return e;
}