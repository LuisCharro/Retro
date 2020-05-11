#pragma once

/*
//He did not have it
//#include <util/typealiases.hpp>
#include <cmp/entity.hpp>

namespace ECS{

//using VecEntities_t = std::vector<Entity_t>;

struct GameContext_t
{
    // virtual pura --> abstract con el = 0

    //Vec_t<Entity_t>
    // virtual const VecEntities_t& GetEntities() const = 0;
    // virtual       VecEntities_t& GetEntities()      = 0;

    
    virtual const Entity_t* GetEntityByID(EntityID_t eid) const = 0;
    virtual       Entity_t* GetEntityByID(EntityID_t eid)      = 0;

    // Returns a pointer to an object or null --> And not to a "deleted or wronly pointed object"
    //virtual const std::optional<std::reference_wrapper<Entity_>> GetEntityByID() const = 0;
    //virtual       std::optional<std::reference_wrapper<Entity_>> GetEntityByID()      = 0;


    virtual const Vec_t<Entity_t>& GetEntities() const = 0;
    virtual       Vec_t<Entity_t>& GetEntities()      = 0;

    virtual const std::vector<PhysicsComponent_t>& GetPhysicsComponents() const = 0;
    virtual       std::vector<PhysicsComponent_t>& GetPhysicsComponents()       = 0;

    virtual const std::vector<RenderComponent_t>& GetRenderComponents() const = 0;
    virtual       std::vector<RenderComponent_t>& GetRenderComponents()       = 0;

    virtual const std::vector<InputComponent_t>& GetInputComponents() const = 0;
    virtual       std::vector<InputComponent_t>& GetInputComponents()       = 0;

    
    virtual ~GameContext_t() = default;
};


}
*/
