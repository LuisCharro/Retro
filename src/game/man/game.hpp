#pragma once

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

// Util
#include <game/util/timer.hpp>

 auto timeCall = [](std::string_view name, auto func){
    GameTImer_t internalTimer;
    func();
    std::cout << "[" << name << "] " << internalTimer.ellapsed() / 1000 << " ";
};

struct GameManager_t
{
    explicit GameManager_t() {

        Render.SetDebugDraw(false);

        timeCall("BIN",[&](){GOFact.LoadLevelBin("assets/levels/level1.bin"); } );
    }

    void Update(){

        GameTImer_t timer;

        Camera.Update(EntityMan);
        Render.Update(EntityMan);
        Input.Update(EntityMan);
        Physics.Update(EntityMan);
        Collision.Update(EntityMan);
        Health.Update(EntityMan);
        Spawn.Update(EntityMan);

        // timeCall("CAM",[&](){ Camera.Update(EntityMan); } );
        // timeCall("REN",[&](){ Render.Update(EntityMan); } );
        // timeCall("INP",[&](){ Input.Update(EntityMan); } );
        // timeCall("PHY",[&](){ Physics.Update(EntityMan); } );
        // timeCall("COL",[&](){ Collision.Update(EntityMan); } );
        // timeCall("HEA",[&](){ Health.Update(EntityMan); } );
        // timeCall("SPA",[&](){ Spawn.Update(EntityMan); } );

        //std::cout << "EXT" << timer.waitUntil_ns(NSPF) << std::endl;
        timer.waitUntil_ns(NSPF);

        //while (!Input.IsKeyPressed(KS_ESCAPE))
        if (Input.IsKeyPressed(KS_ESCAPE))
            m_playing = false;
    }

    bool Alive() {return m_playing;}

    private:

        static constexpr uint32_t kSCRWIDTH {640};
        static constexpr uint32_t kSCRHEIGHT {360};
        static const int KS_ESCAPE = 0xFF1B;

        static constexpr uint64_t FPS { 60 };
        static constexpr uint64_t NSPF { 1000000000UL/FPS };

        ECS::EntityManager_t EntityMan {};
        GameObjectFactory_t GOFact {EntityMan};

        // Systems   
        const RenderSystem_t<ECS::EntityManager_t> Render{kSCRWIDTH,kSCRHEIGHT};
        PhysicsSystem_t<ECS::EntityManager_t> Physics {};
        CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH,kSCRHEIGHT};
        InputSystem_t<ECS::EntityManager_t> Input {};
        SpawnSystem_t<ECS::EntityManager_t> Spawn {};
        CameraSystem_t<ECS::EntityManager_t> Camera {};
        const HealthSystem_t<ECS::EntityManager_t> Health {};
        
        bool m_playing {true};        
};