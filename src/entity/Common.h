#pragma once

#include "common/Common.h"

// Subject to change.
#define ENTT_ID_TYPE    std::uint32_t
#include <entt/entt.hpp>
#include <entt/meta/ctx.hpp>

namespace engine
{
    using EntityID = entt::entity;
    using Handle = entt::handle;    // TODO: Rename or move into a class?

    struct System;
    struct Scene;
    struct Component;
    struct Entity;
    struct Behavior;
}