#pragma once

#include "Common.h"
#include "engine/System.h"

namespace engine
{
    // The global scene that contains all other scenes.
    extern inline struct Scene World;

    /** 
     *  A scene is a collection of entities with  
     *  components assigned to them. All active scenes
     *  are children of World.
     */
    struct Scene : SystemGroup
    {
        Scene* parent = &World;
        entt::registry ents;

        Scene(Scene* parent = &World) : parent(parent) {
            ents.ctx().emplace<Scene*>(this);
        }

        Handle CreateEntity() {
            return Handle(ents, ents.create());
        }

        void DeleteEntity(EntityID id) {
            ents.destroy(id);
        }
    };

    // The global scene that contains all other scenes.
    inline Scene World = Scene(nullptr);
}