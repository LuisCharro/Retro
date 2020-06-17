#include <iostream>

#include "gameobjectfactory.hpp"

ECS::Entity_t& 
GameObjectFactory_t::CreateRectangleEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);
    auto& hc  = m_EntityMan.AddComponent<HealthComponent_t>(e);
    
    ph.x = x; ph.y = y;
    ph.vy = 3; 

    rn.SetDimensions(w,h);

    std::fill(begin(rn.sprite), end(rn.sprite),color);

    cl.boxRoot.box.xLeft  = 1;
    cl.boxRoot.box.xRight = rn.w -1;
    cl.boxRoot.box.yUp    = rn.h -1;
    cl.boxRoot.box.yDown  = 1;

    hc.health = 1;

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateSpriteEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t sprite[]) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);
    auto& hc  = m_EntityMan.AddComponent<HealthComponent_t>(e);

    ph.x = x; ph.y = y;
    ph.vy = 3; 

    rn.SetDimensions(w,h);

    unsigned dataArraySize = w*h;
        
    rn.sprite.resize(dataArraySize);

    std::copy ( sprite, sprite+dataArraySize , rn.sprite.begin());
    //https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping

    cl.boxRoot.box.xLeft  = 1;
    cl.boxRoot.box.xRight = rn.w-1;
    cl.boxRoot.box.yUp    = rn.h-1;
    cl.boxRoot.box.yDown  = 1;

    hc.health = 1;

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateEntity(uint32_t x, uint32_t y, const std::string_view  filename) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);
    auto& h = m_EntityMan.AddComponent<HealthComponent_t>(e);

    rn.transparency = true;

    rn.LoadFromFile(filename);

    ph.x = x; ph.y = y;
    ph.vx = ph.vy = 0; 

    cl.boxRoot.box.xLeft = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp = rn.h;
    cl.boxRoot.box.yDown = 0;

    h.health = 1;
    
    return e;
}

ECS::Entity_t&  
GameObjectFactory_t::CreatePlayer(uint32_t x, uint32_t y) const
{
    // Without the &, I will create a copy and I will not touch the variable    
    auto& e  = CreateEntity(x, y, "assets/ninja.png");
    m_EntityMan.AddComponent<InputComponent_t>(e);

    auto* c = e.getComponent<ColliderComponent_t>();
    
    if (c)
    {
        c->boxRoot.box= {3,37,4,72};
        c->boxRoot.childs =  {
                { { 4, 36, 5, 33}, false,
                    { 
                        { {12,34,20,29}, false, {}}
                    }
                }
            ,   { {10, 30, 34, 52}, false, {}} 
            ,   { { 4,  9, 36, 51}, false, {}} 
            ,   { {31, 35, 36, 51}, false, {}} 
            ,   { { 6, 34, 54, 71}, false, {}} 
        };
    }

    auto* h = e.getComponent<HealthComponent_t>();

    h->health = 100;

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateGhost(uint32_t x, uint32_t y) const
{
    auto& e  = CreateEntity(x, y, "assets/fantasma.png");
    auto* c = e.getComponent<ColliderComponent_t>();
    auto* rn = e.getComponent<RenderComponent_t>();
    auto* ph = e.getComponent<PhysicsComponent_t>();
    auto* h = e.getComponent<HealthComponent_t>();

    c->boxRoot.box.xLeft  = 5;
    c->boxRoot.box.xRight = rn->w - 5;
    c->boxRoot.box.yUp    = rn->h - 5;
    c->boxRoot.box.yDown  = 5;

    ph->vx = 3;
    ph->gravity = 0;
    
    h->health = 50;

    c->mask = ColliderComponent_t::L_Blades;

    return e; 
}

ECS::Entity_t& 
GameObjectFactory_t::CreatePlatform(uint32_t x, uint32_t y) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);    

    rn.transparency = true;

    rn.LoadFromFile("assets/platform.png");

    ph.x = x; ph.y = y;
    ph.vx = ph.vy = 0;
    ph.gravity = 0;

    cl.mask = ColliderComponent_t::L_Platforms;

    cl.boxRoot.box.xLeft = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp = rn.h;
    cl.boxRoot.box.yDown = 0;

    return e; 
}