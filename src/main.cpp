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
#include <game/sys/camera.tpp>

// Managers
#include <ecs/man/entitymanager.tpp>

// Factory
#include <game/util/gameobjectfactory.hpp>


constexpr uint32_t kSCRWIDTH {640};
constexpr uint32_t kSCRHEIGHT {360};
const int KS_ESCAPE = 0xFF1B;

constexpr auto FPS { 60 };
constexpr auto MSPF { 1000ms/FPS };
 
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
        CameraSystem_t<ECS::EntityManager_t> Camera;
        const HealthSystem_t<ECS::EntityManager_t> Health;
        Render.SetDebugDraw(false);

        // Entities
        ECS::EntityManager_t EntityMan;

        GameObjectFactory_t GOFact {EntityMan};

        GOFact.CreateLevel1();        

        using clk = std::chrono::steady_clock;
        auto lastTime = clk::now();

        while (!Input.IsKeyPressed(KS_ESCAPE))
        {
            Camera.Update(EntityMan);
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

