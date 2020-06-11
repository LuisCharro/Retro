#pragma once

#include <cstdint>
#include <ecs/util/typealiases.hpp>

template<typename GameCTX_t>
struct HealthSystem_t
{
    explicit HealthSystem_t() = default;    

    constexpr void Update(GameCTX_t& g) const noexcept;
};
