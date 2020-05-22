#pragma once

//Lots of includes may require a change

#include <cstdint>
#include <vector>
#include <string>
#include <fstream>
#include <iterator> 
#include <iostream>
#include <cstring>

#include <picoPNG/src/picopng.hpp>

#include <ecs/cmp/component.hpp>
#include <ecs/util/typealiases.hpp>

//namespace GAME {

struct RenderComponent_t : ECS::ComponentBase_t<RenderComponent_t>
{
    explicit RenderComponent_t(ECS::EntityID_t eid, uint32_t w0, uint32_t h0) 
    : ComponentBase_t(eid)
    , w (w0)
    , h (h0)
    {
        sprite.resize(w*h); 
    }

    explicit RenderComponent_t(ECS::EntityID_t eid) 
    : ComponentBase_t(eid)
    {
        
    }

    void SetDimensions(uint32_t w0, uint32_t h0);
    void LoadFromFile (const std::string_view filename);
    void InitSpriteFromABGRData(std::vector<unsigned char> pixels);

    std::vector<uint32_t> sprite {};    // 24
    uint32_t w {0}, h {0};              // 8
    bool transparency {false};
};

//Definition --> Outside of the .hpp or I will create several times
// It goes to the cpp
//EntityID_t Entity_t::nextID = 0;

//}