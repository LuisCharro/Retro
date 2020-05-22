#pragma once

#include <cstdint>
#include <memory>

#include <ecs/cmp/entity.hpp>
#include <ecs/util/typealiases.hpp>

//namespace ECS {

    template<typename GameCTX_t>
    struct RenderSystem_t
    {        
        // explicit --> Forces to the user to write the type of the class
        explicit RenderSystem_t(int w, int h);

        ~RenderSystem_t();

        void PrintBackGroundColor(uint32_t screen[], uint32_t w, uint32_t h,  uint32_t iColor) const;

        bool Update(const GameCTX_t& g) const; // It does not modify the data of the class

        void RenderSpriteClipped(const RenderComponent_t& ren, const PhysicsComponent_t& phy) const;
        void DrawAllEntities(const GameCTX_t& g) const noexcept;

        static constexpr uint32_t kR = 0x00FF0000;
        static constexpr uint32_t kG = 0x0000FF00;
        static constexpr uint32_t kB = 0x000000FF;
        
        static constexpr uint32_t sprite[8*8] =
        {
            kG,kG,kG,kG,kG,kG,kG,kG,
            kG,kB,kR,kR,kR,kR,kB,kG,
            kG,kB,kR,kG,kG,kG,kB,kG,
            kG,kB,kR,kR,kG,kG,kB,kG,
            kG,kB,kB,kR,kR,kG,kB,kG,
            kG,kB,kB,kB,kB,kR,kB,kG,
            kG,kB,kR,kR,kR,kR,kB,kG,
            kG,kG,kG,kG,kG,kG,kG,kG,
        };

        private:
            const int m_w {0}, m_h{0};
            //Un constructor por defecto --> Pondra estos valores, si yo no lo indico en ellos
            std::unique_ptr<uint32_t[]>  m_framebuffer { nullptr };            
    };

//}
