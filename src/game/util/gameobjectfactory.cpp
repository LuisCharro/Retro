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
    ph.vy = 3; ph.vx = 3; 

    rn.SetDimensions(w,h);

    std::fill(begin(rn.sprite), end(rn.sprite),color);

    cl.boxRoot.box.xLeft  = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp    = 0;
    cl.boxRoot.box.yDown  = rn.h;

    cl.mask = ColliderComponent_t::L_Platforms;    
    cl.properties = ColliderComponent_t::P_IsSolid;

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
    ph.vy = 3; ph.vx = 3; 

    rn.SetDimensions(w,h);

    unsigned dataArraySize = w*h;
        
    rn.sprite.resize(dataArraySize);

    std::copy ( sprite, sprite+dataArraySize , rn.sprite.begin());
    //https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping

    cl.boxRoot.box.xLeft  = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp    = 0;
    cl.boxRoot.box.yDown  = rn.h;

    cl.mask = ColliderComponent_t::L_Platforms;    
    cl.properties = ColliderComponent_t::P_IsSolid;

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
    cl.boxRoot.box.yUp = 0;
    cl.boxRoot.box.yDown = rn.h;

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
        c->properties = ColliderComponent_t::P_IsPlayer;
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

    h->health = 20;

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

    c->mask = ColliderComponent_t::L_Blades;
    c->properties = ColliderComponent_t::P_Damages;

    c->boxRoot.box.xLeft  = 5;
    c->boxRoot.box.xRight = rn->w - 5;
    c->boxRoot.box.yUp    = 5;
    c->boxRoot.box.yDown  = rn->h - 5;

    ph->vx = 3;
    ph->vy = 3;
    ph->gravity = 0;
    
    h->health = 1;
    h->damageInflicted = 1;
    h->selfDamageOnInfliction = 1;
    
    return e; 
}

ECS::Entity_t& 
GameObjectFactory_t::CreatePlatform(uint32_t x, uint32_t y) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);

    ph.x = x; ph.y = y;    
    ph.gravity = 0;
    ph.friction = 0.90f;

    rn.transparency = true;
    rn.LoadFromFile("assets/platform.png");

    cl.mask = ColliderComponent_t::L_Platforms;    
    cl.properties = ColliderComponent_t::P_IsSolid;
    cl.boxRoot.box.xLeft  = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp    = 0;    
    cl.boxRoot.box.yDown  = rn.h;

    return e; 
}

ECS::Entity_t& 
GameObjectFactory_t::CreateCamera(uint32_t x, uint32_t y, uint32_t w, uint32_t h) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& cam = m_EntityMan.AddComponent<CameraComponent_t>(e);
    auto& phy = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    phy.gravity = 0;

    cam.scrx = x; cam.scry = y;
    cam.w = w; cam.h = h;


    //ph.x = x; ph.y = y;    
    
    return e; 
}

void
GameObjectFactory_t::CreateLevel1() const
{
    CreateCamera(0,0, 640, 360);
    
    CreatePlayer(50,50);
    
    // Level 1 map
    constexpr std::array levelData {
        0b0'0'0'0'0'0'0'0
    ,   0b0'0'0'0'0'0'0'1
    ,   0b0'0'0'0'0'0'0'1
    ,   0b0'1'1'0'0'0'0'1
    ,   0b0'0'0'0'0'0'1'1
    ,   0b0'0'0'0'0'1'1'1
    ,   0b1'1'1'0'1'1'1'1
    ,   0b0'0'0'0'0'0'0'0
    ,   0b0'0'0'0'0'0'0'0
    ,   0b0'1'1'1'1'1'1'1
    };

    uint32_t y {0};
    for(auto row: levelData)
    {
        for(uint32_t x = 0; x < 808; x+= 101)
        {
            if (row & 0x80)
            {
                CreatePlatform(x,y);
            }
            row <<= 1;
        }
        y += 51;
    }   
    
    // Demo entities
    //CreateRectangleEntity(300,10,16,16, 0x00FFFFFF);
    //CreateSpriteEntity(600,50,8,8, sprite);
    
    // Enemies
    CreateGhost(240,100);
    
    CreateSpawner(200,1,
        // Adding as parameter a Lambda with "a function as parameter" 
        [&](const SpawnerComponent_t& spw)
        {
            std::cout << "Using lambda to invokate a method" << std::endl;
            const auto* phy = m_EntityMan.GetRequiredComponent<PhysicsComponent_t>(spw);

            if (!phy) return;

            [[maybe_unused]]auto& e = CreateGhost(phy->x, phy->y);                
        }
    );
}