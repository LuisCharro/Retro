#pragma once

extern "C"
{
    #include "lib/tinyPTC/src/tinyptc.h"
}

#include <iostream>
#include <algorithm>
#include <cmath>

// Systems
#include "render.hpp"

#include <ecs/cmp/entity.hpp>

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>

// Util
//#include <game/util/timer.hpp>

template<typename GameCTX_t>
RenderSystem_t<GameCTX_t>::RenderSystem_t(uint32_t w, uint32_t h):
m_w {w}, 
m_h{h},
m_framebuffer {std::make_unique<uint32_t[]> (m_w*m_h)}    
{        
    ptc_open("window",w,h);
}

template<typename GameCTX_t>
RenderSystem_t<GameCTX_t>::~RenderSystem_t(){
    ptc_close();
}    

//Revised
template<typename GameCTX_t>
bool
RenderSystem_t<GameCTX_t>::Update(const GameCTX_t& g)const
{
    auto screen = m_framebuffer.get();

    // timeCall("BackColor",[&](){ PrintBackGroundColor(screen,m_w, m_h, kBlack); } );
    // timeCall("DrawAllCameras",[&](){ DrawAllCameras(g); } );
    // timeCall("Ptc_Update",[&](){ ptc_update(screen); } );
    // std::cout << "\n";    

    PrintBackGroundColor(screen,m_w, m_h, kBlack);
    DrawAllCameras(g);
    ptc_update(screen);

    return !ptc_process_events();
}

template<typename GameCTX_t>
void 
RenderSystem_t<GameCTX_t>::DrawAllEntities(const GameCTX_t& g) const noexcept
{
    auto& rendcmps = g.template GetComponents<RenderComponent_t>();
    std::for_each(begin(rendcmps),end(rendcmps), [&](const auto&rc)
    {
        // RenderComponent_t must be of an entity that also has a PhysicsComponent_t
        const auto* phy = g.template GetRequiredComponent<PhysicsComponent_t>(rc);

        if (!phy) return;        

        RenderSpriteClipped(rc, *phy);

        // If DebugDraw is active, also draw Horizontalline
        if (m_debugDraw)
        {
            const auto* col = g.template GetRequiredComponent<ColliderComponent_t>(rc);

            if (!col) return;
                        
            DrawBoxTree(col->boxRoot, phy->x, phy->y, m_debugColor);
        }
    });
}

template<typename GameCTX_t>
constexpr void 
RenderSystem_t<GameCTX_t>::DrawAllCameras(const GameCTX_t& g) const
{    
    for (auto& cam : g.template GetComponents<CameraComponent_t>())
    {        
        const auto* phy = g.template GetRequiredComponent<PhysicsComponent_t>(cam);

        if (!phy) return;

        m_currentCam.cam = &cam;
        m_currentCam.phy = phy;

        DrawAllEntities(g);
    }
}

template<typename GameCTX_t>
void
RenderSystem_t<GameCTX_t>::PrintBackGroundColor(uint32_t screen[], uint32_t w, uint32_t h,  uint32_t iColor)const
{
    const auto size = w * h;
    std::fill(screen, screen + size, iColor);
}

template<typename GameCTX_t> 
constexpr void 
RenderSystem_t<GameCTX_t>::RenderSpriteClipped(const RenderComponent_t& ren, const PhysicsComponent_t& phy) const noexcept
{
    bool transparency {ren.transparency};

    // Check Camera Coordiantes
    if (!m_currentCam.cam ||!m_currentCam.phy) return;

    auto& CamScr{ *m_currentCam.cam };
    auto& CamWrl{ *m_currentCam.phy };

    // Sprite ---> World ---> Camera ---> Screen
    //        +pos     -poscam    +poscamscr
    struct {
        BoundingBox_t<float> world  {};
        BoundingBox_t<float> camera {};
        BoundingBox_t<uint32_t> crop {};
        
        struct {
            uint32_t x {}, y{}, w{}, h{};
        } screen;

    } spr;
    
    // Sprite -> World Coors    
    spr.world = {
            phy.x         //xLeft
        ,   phy.x + ren.w //xRight
        ,   phy.y         //yUp
        ,   phy.y + ren.h //yDown
    };

    // Sprite -> Camera Coors
    // CamScr // CamWrl
    spr.camera = {
        spr.world.xLeft  - CamWrl.x, //xLeft
        spr.world.xRight - CamWrl.x, //xRight
        spr.world.yUp    - CamWrl.y, //yUp
        spr.world.yDown  - CamWrl.y  //yDown
    };

    // Sprite Clipping
    // Position of the sprite respecto to the camera
    if (    spr.camera.xRight < 0 || spr.camera.xLeft > CamScr.w ||
            spr.camera.yDown  < 0 || spr.camera.yUp   > CamScr.h)
    {
        return;
    }

    // Sprite Cropping
    // Cropping all contents of the sprite outside of the Camera
    spr.crop = {
            static_cast<uint32_t>(std::round((spr.camera.xLeft  < 0       ) ? -spr.camera.xLeft            : 0))
        ,   static_cast<uint32_t>(std::round((spr.camera.xRight > CamScr.w) ? spr.camera.xRight - CamScr.w : 0))
        ,   static_cast<uint32_t>(std::round((spr.camera.yUp    < 0       ) ? -spr.camera.yUp              : 0))
        ,   static_cast<uint32_t>(std::round((spr.camera.yDown  > CamScr.h) ? spr.camera.yDown  - CamScr.h : 0))
    };

    // Sprite -> Screen Coords (x, y ,w , h)
    spr.screen = {
            static_cast<uint32_t>(std::round(spr.camera.xLeft) + CamScr.scrx    + spr.crop.xLeft)
        ,   static_cast<uint32_t>(std::round(spr.camera.yUp)   + CamScr.scry    + spr.crop.yUp)
        ,   ren.w            - spr.crop.xLeft - spr.crop.xRight
        ,   ren.h            - spr.crop.yUp   - spr.crop.yDown
    };    

    // Render the entity    
    auto* screen   = GetScreenXY(spr.screen.x,spr.screen.y);
    auto sprite_it = begin(ren.sprite) + spr.crop.yUp * ren.w + spr.crop.xLeft;

    // std::cout << "RenderSpriteClipped " << std::endl;

    // std::cout << "spr.world.xLeft,xRight {" << spr.world.xLeft << "," << spr.world.xRight << "}" <<std::endl;
    // std::cout << "spr.world.yUp,yDown    {" << spr.world.yUp   << "," << spr.world.yDown  << "}" <<std::endl;
    // std::cout << " " << std::endl;

    // std::cout << "spr.camera.xLeft,xRight {" << spr.camera.xLeft << "," << spr.camera.xRight << "}" <<std::endl;
    // std::cout << "spr.camera.yUp,yDown    {" << spr.camera.yUp   << "," << spr.camera.yDown  << "}" <<std::endl;
    // std::cout << " " << std::endl;

    // std::cout << "spr.crop.xLeft,xRight   {" << spr.crop.xLeft << "," << spr.crop.xRight << "}" <<std::endl;
    // std::cout << "spr.crop.yUp,yDown      {" << spr.crop.yUp   << "," << spr.crop.yDown  << "}" <<std::endl;
    // std::cout << " " << std::endl;

    // std::cout << "spr.screen.x,y {" << spr.screen.x << "," << spr.screen.y << "}" <<std::endl;
    // std::cout << "spr.screen.h,w {" << spr.screen.h << "," << spr.screen.w << "}" <<std::endl;
    // std::cout << " " << std::endl;
    // std::cin.get();

    while (spr.screen.h--)
    {        
        for (uint32_t i = 0; i < spr.screen.w; ++i)
        {
            // Draw only if transparency != 0 (Not blending)
            if (transparency)
            {
                if (*sprite_it & 0xFF000000)
                     *screen = *sprite_it;
            }
            else
            {
                *screen = *sprite_it;
            }            
            
            ++sprite_it;
            ++screen;            
        }
        
        sprite_it += ren.w - spr.screen.w;
        screen    += m_w   - spr.screen.w;        
    }
}

//Revised
template<typename GameCTX_t>
constexpr void
RenderSystem_t<GameCTX_t>::RenderInScreenBox (uint32_t* screen, const BoundingBox_t<uint32_t>& box, uint32_t pixel) const noexcept
{
    const uint32_t width { box.xRight - box.xLeft };
    uint32_t height { box.yDown - box.yUp };

    while (height--)
    {
        uint32_t x {width};
        while (x--)
        {
            //std::cout << "RenderInScreenBox width: " << width << " height: " << height << " x: " << x << std::endl; 
            *screen = pixel;
            screen++;
        }
        
        screen += m_w - width;
    }
}

template<typename GameCTX_t>
constexpr void
RenderSystem_t<GameCTX_t>::RenderInScreenLine (uint32_t* screen, uint32_t length, uint32_t stride, uint32_t pixel) const noexcept
{
    while (length > 0)
    {
        *screen = pixel;
        --length;
        screen+=stride;
    }
}

template<typename GameCTX_t>
constexpr void
RenderSystem_t<GameCTX_t>::RenderAlignedLineClipped (uint32_t x1, uint32_t x2, uint32_t y, bool yaxis, uint32_t pixel) const noexcept
{
    // This routine is valid either for X or for Y axises.
    // First axis is always called X, Second axis is always called Y
    // However, they represent different axises, depending on the value of yaxis (false=)
    uint32_t infinite { 4*m_w };
    uint32_t maxx { m_w };
    uint32_t maxy { m_h };
    uint32_t stride { 1 };
    uint32_t* screen {nullptr};

    // If the first axis is in Y instead of X switch axis values
    if (yaxis)
    {
        infinite = 4*m_h;
        maxx = m_h;
        maxy = m_w;
        stride = m_w;
    }

    // Ensure that y coordiante is in screen
    if (y >= maxy) return;

    // Define and detect start and end of the line
    uint32_t xini {0}, xend {0};
    
    if (x1 > x2) { xini = x2; xend = x1;}
    else         { xini = x1; xend = x2;}

    // Crop line to screen limits
    if      (xini > infinite) xini =0;
    else if (xini > maxx    ) return;

    if      (xend > infinite) xend =0;
    else if (xend > maxx    ) xend = maxx;

    if      (xini > xend    ) return;

    // Draw the line
    if (yaxis) screen = GetScreenXY(y, xini);
    else       screen = GetScreenXY(xini, y);

    RenderInScreenLine(screen, xend-xini, stride, pixel);
}

template<typename GameCTX_t>
constexpr void
RenderSystem_t<GameCTX_t>::RenderAlignedBoxClipped(BoundingBox_t<uint32_t> box, uint32_t x, uint32_t y, uint32_t pixel) const noexcept
{
    //Crop function
    auto crop = [](uint32_t &val, uint32_t max, uint32_t inf)
    {
        if      (val > inf) val = 0;
        else if (val > max) val = max;
    };

    // Infinite values to be considered negative
    uint32_t xinfinite {4 * m_w};
    uint32_t yinfinite {4 * m_h};

    // Convert Bounding Box to screen coordinates
    box.xLeft += x; box.xRight += x;
    box.yUp   += y; box.yDown  += y;

    //crop line to screen limits
    crop(box.xLeft,  m_w, xinfinite);
    crop(box.xRight, m_w, xinfinite);
    crop(box.yUp,    m_h, yinfinite);
    crop(box.yDown,  m_h, yinfinite);

    // If box is off-screen, do not render
    if (   box.xRight == 0  || box.xLeft == m_w
        || box.yDown  == 0  || box.yUp   == m_h ) return;

    // Render Box
    RenderInScreenBox(GetScreenXY(box.xLeft, box.yUp), box, pixel);
}

template<typename GameCTX_t>
constexpr void 
RenderSystem_t<GameCTX_t>::DrawBox (const BoundingBox_t<uint32_t>& box, uint32_t x, uint32_t y, uint32_t color) const noexcept
{
    uint32_t x1 { x + box.xLeft };
    uint32_t x2 { x + box.xRight };
    uint32_t y1 { y + box.yUp };
    uint32_t y2 { y + box.yDown };

    RenderAlignedLineClipped(x1, x2, y1, false, color);
    RenderAlignedLineClipped(x1, x2, y2, false, color);
    RenderAlignedLineClipped(y1, y2, x1, true, color);
    RenderAlignedLineClipped(y1, y2, x2, true, color);
}

template<typename GameCTX_t>
constexpr void 
RenderSystem_t<GameCTX_t>::DrawBoxTree (const BoundingBoxNode_t& boxNode, uint32_t x, uint32_t y, uint32_t color) const noexcept
{
    if (boxNode.collided)
    {
        RenderAlignedBoxClipped(boxNode.box, x, y, color);
    }   
    else
    {
        DrawBox(boxNode.box, x, y, color);
    }

    for(auto& node : boxNode.childs)
    {
        DrawBoxTree(node, x, y, color >> 1);
    }
}