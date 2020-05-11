#pragma once

// Delete circular refereces
//#include <cmp/entity.hpp>

#include <vector>
#include <memory>
#include <unordered_map>

namespace ECS {

    //Not use forward definitions here
    //struct Entity_t

    //struct Entity_t;

    template<typename T>
    using Vec_t = std::vector<T>;

    template <typename KEY_t, typename VALUE_t>
    using Hast_t = std::unordered_map<KEY_t, VALUE_t>;

    template <typename T>
    using UPtr = std::unique_ptr<T>;
    
    using EntityID_t = std::size_t;
    using ComponentID_t = std::size_t;
    using ComponentTypeID_t = std::size_t;
}