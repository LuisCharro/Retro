#pragma once

extern "C"
{
    #include "lib/tinyPTC/src/tinyptc.h"
}

#include <algorithm>
#include <cmath>

// Systems
#include "render.hpp"

#include <ecs/cmp/entity.hpp>

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>

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

    PrintBackGroundColor(screen,m_w, m_h, kBlack);

    DrawAllEntities(g);

    ptc_update(screen);

    return !ptc_process_events();
}

//Revised but with code after modifications
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
        auto getScreenXY = [&](uint32_t x, uint32_t y) {
        return m_framebuffer.get() + y * m_w + x;
    };

    // Clipping
    uint32_t left_off {0};
    uint32_t up_off {0};

    // Drawing Coordinates and size
    uint32_t x { static_cast<uint32_t>(std::round(phy.x)) };
    uint32_t y { static_cast<uint32_t>(std::round(phy.y)) };
    uint32_t w { ren.w };
    uint32_t h { ren.h };

    bool transparency {ren.transparency};

    // Horizontal clipping rules
    if ( x > m_w)                           //Left clipping
    {
        left_off = 0 - x;
        if (left_off >= w) return;          // Nothing to draw
        x=0;
        w-= left_off;
    }
    else if (x + ren.w >= m_w)              // Right clipping
    {
        uint32_t right_off = x + w - m_w;
        if (right_off >= w ) return;        // Nothing to draw
        w -= right_off;
    }

    // Vertical clipping rules
    if (y >= m_h)                   // Up clipping
    {
        up_off = 0 - y;
        if (up_off >= h) return;    // Nothing to draw
        y = 0;
        h -= up_off;
    } 
    else if (y + ren.h >= m_h)      // Down clipping
    {
        uint32_t down_off = y + h - m_h;
        if (down_off >= h) return; // Nothing to draw
        h -= down_off;
    }

    // Render the entity
    auto screen2 = getScreenXY(x,y);
    auto sprite_it = begin(ren.sprite) + up_off*ren.w + left_off;

    while (h--)
    {
        for (uint32_t i = 0; i < w; ++i)
        {
            // Draw only if transparency != 0 (Not blending)
            if (!transparency)
            {
                *screen2 = *sprite_it;
            }
            else
            {
                if (*sprite_it & 0xFF000000)
                    *screen2 = *sprite_it;
            }
            
            ++sprite_it;
            ++screen2;
        }
        sprite_it += ren.w - w;
        screen2 += m_w -w;
    }
}

//Revised
template<typename GameCTX_t>
constexpr void
RenderSystem_t<GameCTX_t>::RenderInScreenBox (uint32_t* screen, const BoundingBox_t& box, uint32_t pixel) const noexcept
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

//Revised
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

//Revised
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

//Revised
template<typename GameCTX_t>
constexpr void
RenderSystem_t<GameCTX_t>::RenderAlignedBoxClipped(BoundingBox_t box, uint32_t x, uint32_t y, uint32_t pixel) const noexcept
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

//Revised
template<typename GameCTX_t>
constexpr void 
RenderSystem_t<GameCTX_t>::DrawBox (const BoundingBox_t& box, uint32_t x, uint32_t y, uint32_t color) const noexcept
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