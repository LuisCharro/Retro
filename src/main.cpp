extern "C"
{
    #include "lib/tinyPTC/src/tinyptc.h"
}

// General
#include <cstdint>
#include <memory>
#include <iostream>

#include <game/cmp/collider.hpp>

// Systems
#include <game/sys/input.tpp>
#include <game/sys/render.tpp>
#include <game/sys/physics.tpp>
#include <game/sys/collision.tpp>
#include <game/sys/spawn.tpp>

// Managers
#include <ecs/man/entitymanager.hpp>


constexpr uint32_t kSCRWIDTH {640};
constexpr uint32_t kSCRHEIGHT {360};
 
//uint32_t screen[kSCRWIDTH*kSCRHEIGHT];
//uint32_t* screen= { nullptr };
//constexpr uint32_t kWINDOWSIZE {kSCRWIDTH*kSCRHEIGHT};
 
struct Screen_t
{
    Screen_t(uint32_t w, uint32_t h) : screen(new uint32_t [w*h])
    {}
     
    // Screen_t(uint32_t w, uint32_t h)
    // {
    //  screen = new uint32_t [w*h];
    // }
 
    ~Screen_t()
    {
        std::cout << "~Screen_t()" << std::endl;
        delete [] screen;
    }
 
    uint32_t *screen { nullptr };
};

 
//RGBA --> Each pixel 1 Integer of 32 bits
// Expresion const --> It says to the compiler to calculate at compilation time --> I want only the result

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

ECS::Entity_t& CreateRectangleEntity(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color)
{
    auto& e  = EntityMan.CreateEntity();
    auto& rn = EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = EntityMan.AddComponent<ColliderComponent_t>(e);
    
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

ECS::Entity_t& CreateSpriteEntity(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t sprite[])
{
    auto& e  = EntityMan.CreateEntity();
    auto& rn = EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = EntityMan.AddComponent<ColliderComponent_t>(e);

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

ECS::Entity_t& CreateEntity(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y, const std::string_view  filename)
{
    auto& e  = EntityMan.CreateEntity();
    auto& rn = EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = EntityMan.AddComponent<ColliderComponent_t>(e);

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

void CreatePlayer(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y)
{
    // Without the &, I will create a copy and I will not touch the variable    
    auto& e  = CreateEntity(EntityMan, x, y, "assets/ninja.png");
    EntityMan.AddComponent<InputComponent_t>(e);
}

void CreateEnemy(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y)
{
    CreateEntity(EntityMan, x, y, "assets/fantasma.png");    
}

void CreateSpawner(ECS::EntityManager_t& EntityMan, uint32_t x, uint32_t y)
{
    auto& e  = EntityMan.CreateEntity();
    [[maybe_unused]]auto& spw = EntityMan.AddComponent<SpawnerComponent_t>(e);
    auto& ph = EntityMan.AddComponent<PhysicsComponent_t>(e);

    ph.x = x; ph.y = y;
}
 
int main(int argc, char const *argv[])
{
    try
    {
        // Systems
        const RenderSystem_t<ECS::EntityManager_t> Render{kSCRWIDTH,kSCRHEIGHT};
        PhysicsSystem_t<ECS::EntityManager_t> Physics;
        CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH,kSCRHEIGHT};
        InputSystem_t<ECS::EntityManager_t> Input;
        SpawnSystem_t<ECS::EntityManager_t> Spawn;

        // Entities
        ECS::EntityManager_t EntityMan;

        CreateRectangleEntity(EntityMan, 10,10,16,16, 0x00FFFFFF);
        CreateSpriteEntity(EntityMan, 50,50,8,8, sprite);

        CreatePlayer(EntityMan, 400,100);
        CreateEnemy(EntityMan, 100,100);

        CreateSpawner(EntityMan,200,1);
        
        // Main Loop
        while (Render.Update(EntityMan))
        {
            Input.Update(EntityMan);
            Physics.Update(EntityMan);
            Collision.Update(EntityMan);
            Spawn.Update(EntityMan);
        }        
    }
    catch (const char* s)
    {
        std::cout << "Capturada: " << s << std::endl;
    }
    catch (const std::system_error& e) {
        std::clog << e.what() << " (" << e.code() << ")" << std::endl;
    }
    catch (std::exception &e)
    {
        std::cout << "Capturada" << std::endl;
    }
 
    return 0;
}
