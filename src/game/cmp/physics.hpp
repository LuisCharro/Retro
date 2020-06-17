#pragma once

#include <cstdint>
#include <array>
#include <ecs/cmp/component.hpp>

struct PhysicsComponent_t : public ECS::ComponentBase_t<PhysicsComponent_t>
{
    //The constructor is incomplete once we had added herency
    //explicit PhysicsComponent_t () = default;

    explicit PhysicsComponent_t (ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {
    }

    //constexpr --> Inline + posibility to inicialize calling a constexpr function 
    static constexpr int32_t KGravity {1};

    static constexpr int32_t KMinVy { -6};
    static constexpr int32_t KMaxVy {  6};

    static constexpr int32_t KMinVx { -4};
    static constexpr int32_t KMaxVx {  4};
    static constexpr uint8_t KTimesVyIs0ToJump { 3};

    // 15 -->Non-type template parameter
    //static const std::array<int,15> jumpTable 
    static constexpr std::array jumpTable 
    { -7, -7, -7, -6, -6, -6, -5, -5 , -5, -4, -4, -4, -3, -3, -2 };

    uint32_t x {0}, y {0};
    int32_t vx {0}, vy {0};

    int32_t gravity { KGravity };

    uint8_t jumpIdx { jumpTable.size() };
    uint8_t timesVyIs0 { 0 };
};