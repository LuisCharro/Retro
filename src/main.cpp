extern "C"
{
    #include "lib/tinyPTC/src/tinyptc.h"
}

// General
#include <cstdint>
#include <memory>
#include <iostream>
#include <thread>

#include <game/cmp/collider.hpp>

// Systems
#include <game/sys/input.tpp>
#include <game/sys/render.tpp>
#include <game/sys/physics.tpp>
#include <game/sys/collision.tpp>
#include <game/sys/spawn.tpp>
#include <game/sys/health.tpp>

// Managers
#include <ecs/man/entitymanager.tpp>

// Factory
#include <game/util/gameobjectfactory.hpp>


constexpr uint32_t kSCRWIDTH {640};
constexpr uint32_t kSCRHEIGHT {360};
const int KS_ESCAPE = 0xFF1B;

constexpr auto FPS { 60 };
constexpr auto MSPF { 1000ms/FPS };
 
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
        const HealthSystem_t<ECS::EntityManager_t> Health;
        Render.SetDebugDraw(true);

        // Entities
        ECS::EntityManager_t EntityMan;

        GameObjectFactory_t GOFact {EntityMan};

        // Platforms
        GOFact.CreatePlatform(250,270);

        GOFact.CreatePlatform(100,343);
        GOFact.CreatePlatform(201,343);
        GOFact.CreatePlatform(301,343);
        GOFact.CreatePlatform(401,343);

        // Player
        GOFact.CreatePlayer(100,266);
        
        // Demo entities
        GOFact.CreateRectangleEntity(10,10,16,16, 0x00FFFFFF);
        GOFact.CreateSpriteEntity(100,50,8,8, sprite);
        
        // Enemies
        GOFact.CreateGhost(300,100);

        GOFact.CreateSpawner(200,1,
            // Adding as parameter a Lambda with "a function as parameter" 
            [&](const SpawnerComponent_t& spw)
            {
                std::cout << "Using lambda to invokate a method" << std::endl;
                const auto* phy = EntityMan.GetRequiredComponent<PhysicsComponent_t>(spw);

                if (!phy) return;

                [[maybe_unused]]auto& e = GOFact.CreateGhost(phy->x, phy->y);                
            }
        );       

        using clk = std::chrono::steady_clock;
        auto lastTime = clk::now();

        while (!Input.IsKeyPressed(KS_ESCAPE))
        {
            Render.Update(EntityMan);
            Input.Update(EntityMan);
            Physics.Update(EntityMan);
            Collision.Update(EntityMan);
            Health.Update(EntityMan);
            Spawn.Update(EntityMan);

            auto interval = clk::now()-lastTime;

            if (interval < MSPF)
                std::this_thread::sleep_for(MSPF - interval);

            lastTime = clk::now();
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