#pragma once

#include <concepts>

#include "common/Traits.h"

#include "Common.h"
#include "Scene.h"
#include "Component.h"

namespace engine
{
    struct Entity
    {
        // The default scene for entities
        static constexpr Scene& World = Scene::World;

        // Handle: registry ptr + entity ID
        Handle handle;

        // Create a new entity
        Entity() : handle(World.CreateEntity()) {}
        Entity(Handle& handle) : handle(handle) {}
        
        Scene& GetScene() const {
            return *handle.registry()->ctx<Scene*>();
        }

        template <class C>
        C& AddComponent() {
            // Add required components, if any
            if constexpr (derived_from_template<C, RequireComponents>) {
                C::AddRequiredComponents(handle);
            }

            // Create component instance
            C& component = handle.emplace<C>();
            
            // If this component is an entity, then share our handle with it
            if constexpr (std::derived_from<C, Entity>) {
                static_cast<Entity&>(component).handle = handle;
            }
            
            if constexpr (std::derived_from<C, Behavior>) {

            }
            
            return component;
        }

        template <class C>
        C& GetComponent() {
            return handle.get<C>();
        }

        template <class C>
        void RemoveComponent() {
            if constexpr (std::derived_from<C, Behavior>) {

            }
            handle.erase<C>();
        }
    };

    // A pure Entity cannot be a component.
    template<> Entity& Entity::AddComponent<Entity>() = delete;
    template<> Entity& Entity::GetComponent<Entity>() = delete;
    template<> void Entity::RemoveComponent<Entity>() = delete;
}