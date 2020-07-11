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

// Factory
#include <game/util/gameobjectfactory.hpp>

// Util
#include <game/util/timer.hpp>

// Managers
#include <ecs/man/entitymanager.tpp>
#include <game/man/state.hpp>

#include <X11/keysym.h>

struct GameManager_t : StateBase_t
{
    explicit GameManager_t(StateManager_t& sm) : SM{sm} {

        Render.SetDebugDraw(false);

        timeCall("BIN",[&](){GOFact.LoadLevelBin("assets/levels/level1.bin"); } );
    }

    void Update() final {

        GameTimer_t timer;

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

        timer.waitUntil_ns(NSPF);
        //std::cout << "EXT " << std::setw(6) << std::setfill('0') << timer.waitUntil_ns(NSPF) / 1000 << "\n";
        timer.start();

        if (Input.IsKeyPressed(XK_Escape))
            m_playing = false;

        if (Input.IsKeyPressed(XK_p))
            SM.PushState<PauseState_t>();
    }

    bool Alive() final {return m_playing;}

    private:

        static constexpr uint32_t kSCRWIDTH {640};
        static constexpr uint32_t kSCRHEIGHT {360};        

        static constexpr uint64_t FPS { 60 };
        static constexpr uint64_t NSPF { 1000000000UL/FPS };

        StateManager_t& SM;

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