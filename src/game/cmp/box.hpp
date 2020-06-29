#pragma once
#include <cstdint>
#include <vector>

template <typename Data_t>
struct BoundingBox_t
{
    Data_t xLeft {0}, xRight {0};
    Data_t yUp {0}, yDown {0}; 
};

struct BoundingBoxNode_t
{
    BoundingBox_t<uint32_t> box;
    bool collided {false};
    std::vector<BoundingBoxNode_t> childs;
};
