
#include <game/cmp/render.hpp>
#include <tuple>

#include <string_view>


//namespace ECS {
    
    void RenderComponent_t::SetDimensions(uint32_t w0, uint32_t h0)
    {
        w = w0;
        h = h0;
        sprite.resize(w*h); 
    }

    //Auto return tyoe
    auto 
    LoadPNGFileIntoVector(const std::string_view filename)
    {
        std::ifstream file (filename.data(), std::ios::binary);

        if (!file)
        {   
            throw std::system_error(errno, std::system_category(), "failed to open ");            
        }

        std::vector<unsigned char> filevec(
             std::istreambuf_iterator<char>{file},
             std::istreambuf_iterator<char>{} // default construction goes to the end
             );

        std::vector<unsigned char> pixels {};
        unsigned long dw {0}, dh {0};
        decodePNG(pixels,dw,dh, filevec.data(), filevec.size());

        //Class Template Argument Deduction (CTAD) 
        return std::tuple {dw, dh, pixels};
    } 

    void RenderComponent_t::InitSpriteFromABGRData(std::vector<unsigned char> pixels)
    {
        // sprite.data --> nullptr by default
        sprite.reserve(pixels.size()/4); // Change the size and fill with 0s

        // Easy method to copy the memory but problems with colors
        //std::memcpy(sprite.data(),pixels.data(),pixels.size()); // And the we copy the data        

        for (auto p = pixels.begin(); p != pixels.end(); p += 4 )
        {
            uint32_t pixel = 
                static_cast<uint32_t>(*(p+0)) << 16
            |   static_cast<uint32_t>(*(p+1)) << 8
            |   static_cast<uint32_t>(*(p+2)) 
            |   static_cast<uint32_t>(*(p+3)) << 24;

            sprite.push_back(pixel);
        }
    }

    void  RenderComponent_t::LoadFromFile (const std::string_view filename)
    {
        //Structure Bindings
         auto [dw, dh, pixels] = LoadPNGFileIntoVector(filename);
         w = dw;  h = dh;
        
        InitSpriteFromABGRData(pixels);
    }
//}