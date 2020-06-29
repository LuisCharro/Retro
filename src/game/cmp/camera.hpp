#pragma once

#include <cstdint>

#include <ecs/cmp/component.hpp>

struct CameraComponent_t : public ECS::ComponentBase_t<CameraComponent_t>
{
    explicit CameraComponent_t (ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {
    }

    static constexpr uint32_t KDefaultWidth  { 640 };
    static constexpr uint32_t KDefaultHeight { 360 };

    // Proyection from Sprite --> to the World --> to Camera --> to Screen
    uint32_t scrx {0}, scry {0};    

    uint32_t w {KDefaultWidth}, h {KDefaultHeight};    
};