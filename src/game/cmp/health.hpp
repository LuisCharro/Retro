#pragma once

#include <cstdint>

#include <ecs/cmp/component.hpp>

struct HealthComponent_t : public ECS::ComponentBase_t<HealthComponent_t>
{
    explicit HealthComponent_t (ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {
    }

    uint32_t selfDamageOnInfliction { 1 };
    uint32_t damageInflicted { 1 };
    uint32_t damage { 0 }; // Acumulated damage from collisions
    uint32_t health { 1 };
};