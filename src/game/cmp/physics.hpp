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
    // Change to linear coordinates from int32_t or uint32_t to float
    static constexpr float KGravity   {  0.5f };

    static constexpr float KMinVy     { -6.0f };
    static constexpr float KMaxVy     {  6.0f };

    static constexpr float KMinVx     { -5.0f };
    static constexpr float KMaxVx     {  5.0f };

    static constexpr float KStdAx     {  0.6f };//{  1.0f };

    static constexpr float KMinVyForNotOnPlatform { 2.5*KGravity };

    // 15 -->Non-type template parameter
    //static const std::array<int,15> jumpTable 
    static constexpr std::array jumpTable 
    { -7, -7, -7, -6, -6, -6, -5, -5, -5, -4, -4, -4, -3, -3, -2 };    
    
    // Change to linear coordinates from int32_t or uint32_t to float
    float x  { 0 }, y  { 0 };
    float vx { 0 }, vy { 0 };
    float ax { 0 };
    float friction { 1.0f };
    float gravity { KGravity };

    //TODO: Do not use float use a type with fix 

    uint8_t jumpIdx    { jumpTable.size() };    
    bool onPlatform { false }; //Fast solution; not good to have a boolean inside an structure
};