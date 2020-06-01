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

// Factory
#include <game/util/gameobjectfactory.hpp>


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

        GameObjectFactory_t GOFact {EntityMan};

        GOFact.CreateRectangleEntity(10,10,16,16, 0x00FFFFFF);
        GOFact.CreateSpriteEntity(50,50,8,8, sprite);

        GOFact.CreatePlayer(400,100);
        GOFact.CreateGhost(100,100);

        GOFact.CreateSpawner(200,1,
            // Adding as parameter a Lambda with "a function as parameter" 
            [&](const SpawnerComponent_t& spw)
            {
                std::cout << "Using lambda to invokate a method" << std::endl;
                const auto* spawnerEntity = EntityMan.GetEntityByID(spw.GetEntityID());

                if (!spawnerEntity) return;
                const auto* phy = spawnerEntity->getComponent<PhysicsComponent_t>();
                if (!phy) return;

                [[maybe_unused]]auto& e = GOFact.CreateGhost(phy->x, phy->y);
            }
        );
        
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