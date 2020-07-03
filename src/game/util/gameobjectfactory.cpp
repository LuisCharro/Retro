#include <iostream>
#include <fstream>
#include <lib/picoJSON/picojson.hpp>


#include "gameobjectfactory.hpp"

ECS::Entity_t& 
GameObjectFactory_t::CreateRectangleEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);
    auto& hc  = m_EntityMan.AddComponent<HealthComponent_t>(e);
    
    ph.x = x; ph.y = y;
    ph.vy = 3; ph.vx = 3; 

    rn.SetDimensions(w,h);

    std::fill(begin(rn.sprite), end(rn.sprite),color);

    cl.boxRoot.box.xLeft  = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp    = 0;
    cl.boxRoot.box.yDown  = rn.h;

    cl.mask = ColliderComponent_t::L_Platforms;    
    cl.properties = ColliderComponent_t::P_IsSolid;

    hc.health = 1;

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateSpriteEntity(uint32_t x, uint32_t y, uint32_t w, uint32_t h, const uint32_t sprite[]) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);
    auto& hc  = m_EntityMan.AddComponent<HealthComponent_t>(e);

    ph.x = x; ph.y = y;
    ph.vy = 3; ph.vx = 3; 

    rn.SetDimensions(w,h);

    unsigned dataArraySize = w*h;
        
    rn.sprite.resize(dataArraySize);

    std::copy ( sprite, sprite+dataArraySize , rn.sprite.begin());
    //https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping

    cl.boxRoot.box.xLeft  = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp    = 0;
    cl.boxRoot.box.yDown  = rn.h;

    cl.mask = ColliderComponent_t::L_Platforms;    
    cl.properties = ColliderComponent_t::P_IsSolid;

    hc.health = 1;

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateEntity(uint32_t x, uint32_t y, const std::string_view  filename) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);
    auto& h = m_EntityMan.AddComponent<HealthComponent_t>(e);

    rn.transparency = true;
    rn.LoadFromFile(filename);

    ph.x = x; ph.y = y;
    ph.vx = ph.vy = 0; 

    cl.boxRoot.box.xLeft = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp = 0;
    cl.boxRoot.box.yDown = rn.h;

    h.health = 1;
    
    return e;
}

ECS::Entity_t&  
GameObjectFactory_t::CreatePlayer(uint32_t x, uint32_t y) const
{
    // Without the &, I will create a copy and I will not touch the variable    
    auto& e  = CreateEntity(x, y, "assets/ninja.png");
    m_EntityMan.AddComponent<InputComponent_t>(e);

    auto* c = e.getComponent<ColliderComponent_t>();
    
    if (c)
    {
        //xor
        c->mask ^= ColliderComponent_t::L_Boundaries;
        c->properties = ColliderComponent_t::P_IsPlayer;
        c->boxRoot.box= {3,37,4,72};
        c->boxRoot.childs =  {
                { { 4, 36, 5, 33}, false,
                    { 
                        { {12,34,20,29}, false, {}}
                    }
                }
            ,   { {10, 30, 34, 52}, false, {}} 
            ,   { { 4,  9, 36, 51}, false, {}} 
            ,   { {31, 35, 36, 51}, false, {}} 
            ,   { { 6, 34, 54, 71}, false, {}} 
        };
    }

    auto* h = e.getComponent<HealthComponent_t>();

    h->health = 20;

    return e;
}

ECS::Entity_t& 
GameObjectFactory_t::CreateGhost(uint32_t x, uint32_t y) const
{
    auto& e  = CreateEntity(x, y, "assets/fantasma.png");
    auto* c = e.getComponent<ColliderComponent_t>();
    auto* rn = e.getComponent<RenderComponent_t>();
    auto* ph = e.getComponent<PhysicsComponent_t>();
    auto* h = e.getComponent<HealthComponent_t>();

    // Or
    c->mask = ColliderComponent_t::L_Blades | ColliderComponent_t::L_Boundaries;
    c->properties = ColliderComponent_t::P_Damages;

    c->boxRoot.box.xLeft  = 5;
    c->boxRoot.box.xRight = rn->w - 5;
    c->boxRoot.box.yUp    = 5;
    c->boxRoot.box.yDown  = rn->h - 5;

    ph->vx = 3;
    ph->vy = 3;
    ph->gravity = 0;
    
    h->health = 1;
    h->damageInflicted = 1;
    h->selfDamageOnInfliction = 1;
    
    return e; 
}

ECS::Entity_t& 
GameObjectFactory_t::CreatePlatform(uint32_t x, uint32_t y) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& rn = m_EntityMan.AddComponent<RenderComponent_t>(e);
    auto& ph = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntityMan.AddComponent<ColliderComponent_t>(e);

    ph.x = x; ph.y = y;    
    ph.gravity = 0;
    ph.friction = 0.90f;

    rn.transparency = true;
    rn.LoadFromFile("assets/platform.png");

    cl.mask = ColliderComponent_t::L_Platforms;    
    cl.properties = ColliderComponent_t::P_IsSolid;
    cl.boxRoot.box.xLeft  = 0;
    cl.boxRoot.box.xRight = rn.w;
    cl.boxRoot.box.yUp    = 0;    
    cl.boxRoot.box.yDown  = rn.h;

    return e; 
}

ECS::Entity_t& 
GameObjectFactory_t::CreateCamera(uint32_t x, uint32_t y, uint32_t w, uint32_t h, ECS::EntityID_t followEID) const
{
    auto& e  = m_EntityMan.CreateEntity();
    auto& cam = m_EntityMan.AddComponent<CameraComponent_t>(e);
    auto& phy = m_EntityMan.AddComponent<PhysicsComponent_t>(e);
    phy.gravity = 0;

    cam.scrx = x; cam.scry = y;
    cam.w = w; cam.h = h;
    cam.followEntityID = followEID;

    //ph.x = x; ph.y = y;    
    
    return e; 
}

void
GameObjectFactory_t::CreateLevel1() const
{
    // Demo entities
    //CreateRectangleEntity(300,10,16,16, 0x00FFFFFF);
    //CreateSpriteEntity(600,50,8,8, sprite);
    
    // Enemies
    CreateGhost(240,100);
    
    //auto& sp = CreateSpawner(200,1,
    CreateSpawner(200,1,
        // Adding as parameter a Lambda with "a function as parameter" 
        [&](const SpawnerComponent_t& spw)
        {
            std::cout << "Using lambda to invokate a method" << std::endl;
            const auto* phy = m_EntityMan.GetRequiredComponent<PhysicsComponent_t>(spw);

            if (!phy) return;

            [[maybe_unused]]auto& e = CreateGhost(phy->x, phy->y);                
        }
    );

    auto& pl { CreatePlayer( 100,100) };   
    CreateCamera(  0,  0, 640, 360, pl.getEntityID());
    //CreateCamera(410,100, 230, 160, sp.getEntityID());
}

void
GameObjectFactory_t::Json2Bin(std::string_view jsonpath, std::string_view binpath) const
{
    // Open JSON file
    namespace pj = picojson;
    std::ifstream fileJson (jsonpath.data());

    if (!fileJson)
    {
        throw std::runtime_error("Can't open JSON file");
    }

    // Open Bin file (Overwrite)
    std::ofstream fileBin (binpath.data(), std::ofstream::binary | std::ofstream::trunc);

    if (!fileBin)
    {
        throw std::runtime_error("Can't create BIN file");
    }

    // Read JSON
    pj::value json;
    fileJson >> json;

    std::string err {pj::get_last_error()};

    if (!err.empty())
    {
        throw std::runtime_error(err);
    }    
   
    const auto& root { json.get<pj::object>()};
    const auto& h    { static_cast<uint32_t>(root.at("height").get<double>())};
    const auto& w    { static_cast<uint32_t>(root.at("width").get<double>())};
    const auto& map  { root.at("map").get<pj::array>()};

    if (map.size() != w*h)
    {
        throw std::runtime_error("Map size error");
    }

    // Write to binary
    fileBin.write(reinterpret_cast<const char*> (&w), sizeof w);
    fileBin.write(reinterpret_cast<const char*> (&h), sizeof h);

    for(auto& elem: map)
    {
        const auto& tile { static_cast<uint8_t>(elem.get<double>())};
        fileBin.write(reinterpret_cast<const char*> (&tile), sizeof tile);
    }
}

void
GameObjectFactory_t::LoadLevelJSON(std::string_view filepath) const
{
    // Open JSON file
    namespace pj = picojson;
    std::ifstream file (filepath.data());

    if (!file)
    {
        throw std::runtime_error("Can't open JSON file");
    }

    // Read JSON
    pj::value json;
    file >> json;

    std::string err {pj::get_last_error()};

    if (!err.empty())
    {
        throw std::runtime_error(err);
    }
    
    const auto& root {json.get<pj::object>()};
    const auto& h    {root.at("height").get<double>()};
    const auto& w    {root.at("width").get<double>()};
    const auto& map  {root.at("map").get<pj::array>()};

    if (map.size() != w*h)
    {
        throw std::runtime_error("Map size error");
    }

    uint32_t x {0}, y {0};

    for(auto& elem: map)
    {
        const auto& tile { static_cast<bool>(elem.get<double>())};
        if (tile) CreatePlatform(101*x,50*y);
        if (++x == w) {x=0; ++y;}
    }   

    CreateLevel1();
}

void
GameObjectFactory_t::LoadLevelBin(std::string_view filepath) const
{
    // Open BIN file
    std::ifstream file (filepath.data(), std::ifstream::binary);

    if (!file)
    {
        throw std::runtime_error("Can't open BIN file for read");
    }

    // Calculate file lenght
    std::size_t lenght = [&]()
    {
        file.seekg(0, std::ifstream::end);
        auto l { file.tellg()};
        file.seekg(0, std::ifstream::beg);
        return l;
    }(); //<-- Inmediate invoke

    if (lenght < 8) throw std::runtime_error("Binary file corrupt. Size too small");

    //Read whole file at once
    std::vector<char> filemem(lenght);
    auto* pfilemem = filemem.data();
    file.read(pfilemem, lenght);

    uint32_t h{}, w{};
    std::memcpy(&w, pfilemem + 0, sizeof w);
    std::memcpy(&h, pfilemem + 4, sizeof h);
    //C++ 20 std::bitcast

    // Not good --> There are not uint32_t inside filemem (std::vector<char>)
    // w = *reinterpret_cast<uint32_t*>(filemem + 0);
    // h = *reinterpret_cast<uint32_t*>(filemem + 4);

    if (w*h != (lenght -8)) throw std::runtime_error("Bad size in Bin file level");

    pfilemem +=8;

    for(uint32_t y=0; y <h; ++y)
    {
        for(uint32_t x=0; x <w; ++x)
        {
            if (*pfilemem) CreatePlatform(101*x,50*y);
            pfilemem++;
        }    
    }

    CreateLevel1();
}