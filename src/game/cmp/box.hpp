#pragma once
#include <cstdint>
#include <vector>

struct BoundingBox_t
{
    uint32_t xLeft {0}, xRight {0};
    uint32_t yUp {0}, yDown {0}; 
};

struct BoundingBoxNode_t
{
    BoundingBox_t box;
    std::vector<BoundingBoxNode_t> childs;
};
