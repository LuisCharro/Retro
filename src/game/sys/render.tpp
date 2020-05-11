
extern "C"
{
    #include "lib/tinyPTC/src/tinyptc.h"
}

// Systems
#include "render.hpp"

#include <ecs/cmp/entity.hpp>

// Components
#include <game/cmp/physics.hpp>
#include <game/cmp/render.hpp>

#include <algorithm>

//namespace GAME {

    template<typename GameCTX_t>
    RenderSystem_t<GameCTX_t>::RenderSystem_t(int w, int h):
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

    template<typename GameCTX_t>
    void
    RenderSystem_t<GameCTX_t>::PrintBackGroundColor(uint32_t screen[], uint32_t w, uint32_t h,  uint32_t iColor)const
    {
        const auto size = w * h;
        std::fill(screen, screen + size,iColor);
    }

    template<typename GameCTX_t>
    bool
    RenderSystem_t<GameCTX_t>::Update(const GameCTX_t& g)const
    {
        auto screen = m_framebuffer.get();

        PrintBackGroundColor(screen,m_w, m_h, 0x00FF0000);

        // DrawAllEntities(g.GetEntities()); // Before
        DrawAllEntities(g);

        ptc_update(m_framebuffer.get());

        return !ptc_process_events();
    }

    template<typename GameCTX_t>
    void 
    RenderSystem_t<GameCTX_t>::RenderSpriteClipped(const RenderComponent_t& ren, const PhysicsComponent_t& phy) const
    {
         auto getScreenXY = [&](uint32_t x, uint32_t y) {
            return m_framebuffer.get() + y * m_w + x;
        };

        // Clipping
        uint32_t left_off {0};
        uint32_t up_off {0};

        // Drawing Coordinates and size
        uint32_t x { phy.x };
        uint32_t y { phy.y };
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

    template<typename GameCTX_t>
    void 
    RenderSystem_t<GameCTX_t>::DrawAllEntities(const GameCTX_t& g) const noexcept
    {
        // Lambda ->  With [&] gets by reference all from the context
        // [=] por copia
        //                                      Here add mutable to remove the const 
        
        auto drawEntity = [&](const RenderComponent_t& rc)  {

            auto eptr = g.GetEntityByID(rc.GetEntityID());
            
            if (eptr)
            {
                auto* phy = eptr->template getComponent<PhysicsComponent_t>();            

                if (phy)
                {
                    RenderSpriteClipped(rc, *phy);
                    
                    // auto screen = getScreenXY(phy->x,phy->y);
                    // auto sprite_it = begin(rc.sprite);

                    // for (uint32_t y = 0; y < rc.h; ++y)
                    // {
                    //     std::copy(sprite_it,sprite_it + rc.w,screen);
                    //     sprite_it += rc.w;
                    //     screen += m_w;
                    // }

                    /*
                        If matrix has size, n by m [i.e. i goes from 0 to (n-1) and j from 0 to (m-1) ], then:
                    
                            matrix[ i ][ j ] = array[ i*m + j ].
                    
                                |-------- 5 ---------|
                        Row      ______________________   _ _
                        0       |0    1    2    3    4 |   |
                        1       |5    6    7    8    9 |   3
                        2       |10   11   12   13   14|  _|_
                                |______________________|
                        Column   0    1    2    3    4
                    
                        For higher dimension, this idea generalizes, i.e. for a 3D matrix L by N by M:
                    
                            matrix[ i ][ j ][ k ] = array[ i*(N*M) + j*M + k ]    
                    
                        //More info
                        http://www.cplusplus.com/doc/tutorial/arrays/
                        */
                    
                }
            }
        };

        // referencia is an alias to an object --> It comes the object
        // wihtout & I will use the operator = with the new variable nencmps
        // We need to use & and it is like I use the vector with other name
        // It is not a pointer its the object
        // You can see more allocations with Valgrind (the copy destroy --> so the app does not grow)    
        auto& rencmps  = g.template GetComponents<RenderComponent_t>();

        //for (auto& inp : g.template GetComponents<RenderComponent_t>()) 
        // example
        //auto& otherReference = rencmps;

        //Before
        //std::for_each(begin(entities),end(entities),drawEntity);
        std::for_each(begin(rencmps),end(rencmps),drawEntity);
    }
//}