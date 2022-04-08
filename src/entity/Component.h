#pragma once

#include "Common.h"

namespace engine
{
    struct Component {};

    // Attribute to require other components. Must be publically inherited.
    template <class... Components>
    struct RequireComponents
    {
    protected:
        friend struct Entity;
        static constexpr void AddRequiredComponents(Handle& h) {
            //(std::printf("%s\n", typeid(Components).name()), ...);
            (h.emplace<Components>(), ...);
        }
    };

    // RequireComponent: Alias for RequireComponents
    template <class... Components>
    using RequireComponent = RequireComponents<Components...>;

}