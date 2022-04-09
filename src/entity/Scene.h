#pragma once

#include "Common.h"
#include "engine/System.h"

namespace engine
{
    struct Scene : SystemGroup
    {
        // The global scene that contains all other scenes.
        static Scene World;

        Scene* parent = &World;
        entt::registry ents;

        Scene(Scene* parent = &World) : parent(parent) {
            ents.set<Scene*>(this);
        }

        Handle CreateEntity() {
            return Handle(ents, ents.create());
        }

        void DeleteEntity(EntityID id) {
            ents.destroy(id);
        }
    };

    // The global scene that contains all other scenes.
    inline Scene Scene::World = Scene(nullptr);
}