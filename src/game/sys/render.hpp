#pragma once

#include <cstdint>
#include <memory>

#include <ecs/cmp/entity.hpp>
#include <ecs/util/typealiases.hpp>

struct RenderComponent_t;
struct PhysicsComponent_t;
struct BoundingBox_t;

template<typename GameCTX_t>
struct RenderSystem_t
{        
    // explicit --> Forces to the user to write the type of the class
    explicit RenderSystem_t(uint32_t w, uint32_t h);

    ~RenderSystem_t();        

    bool Update(const GameCTX_t& g) const; // It does not modify the data of the class    
    void DrawAllEntities(const GameCTX_t& g) const noexcept;

    constexpr void SetDebugDraw(bool v) const noexcept {m_debugDraw = v;}
    constexpr void SetDebugColor(uint32_t c) const noexcept {m_debugColor = c;}
    
    [[nodiscard]] constexpr bool DebugDraw() const noexcept {return m_debugDraw;}
    [[nodiscard]] constexpr bool DebugColor() const noexcept {return m_debugColor;}

    private:
        // Helper functions
        void PrintBackGroundColor(uint32_t screen[], uint32_t w, uint32_t h,  uint32_t iColor) const;
        constexpr uint32_t* GetScreenXY(uint32_t x, uint32_t y) const {return m_framebuffer.get()+ y * m_w + x;};
        constexpr void RenderSpriteClipped(const RenderComponent_t& ren, const PhysicsComponent_t& phy) const noexcept;
        constexpr void RenderInScreenBox (uint32_t* screen, const BoundingBox_t& box, uint32_t pixel) const noexcept;
        constexpr void RenderInScreenLine (uint32_t* screen, uint32_t length, uint32_t stride, uint32_t pixel) const noexcept;
        constexpr void RenderAlignedLineClipped (uint32_t x1, uint32_t x2, uint32_t y, bool yaxis, uint32_t pixel) const noexcept;
        constexpr void RenderAlignedBoxClipped (BoundingBox_t box, uint32_t x, uint32_t y, uint32_t pixel) const noexcept;
        constexpr void DrawBox (const BoundingBox_t& box, uint32_t x, uint32_t y, uint32_t color) const noexcept;
        constexpr void DrawBoxTree (const BoundingBoxNode_t& boxNode, uint32_t x, uint32_t y, uint32_t color) const noexcept;

        static constexpr uint32_t kR = 0x00FF0000; // Red
        static constexpr uint32_t kG = 0x0000FF00; // Green
        static constexpr uint32_t kB = 0x000000FF; // Blue

        static constexpr uint32_t kBlack = 0x00000000;      // Black
        static constexpr uint32_t kRedAlfa = 0xFFFF0000;    // Red + Alfa

        mutable bool m_debugDraw  {true};
        mutable uint32_t m_debugColor {kRedAlfa};

        const uint32_t m_w {0}, m_h{0};
        //A default constructor will add those values (If, I do not indicate diferently on they)
        std::unique_ptr<uint32_t[]>  m_framebuffer { nullptr };            
};