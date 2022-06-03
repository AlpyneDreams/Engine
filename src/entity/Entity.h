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
        // Handle: registry ptr + entity ID
        Handle handle;

        // Create a new entity
        Entity() : handle(World.CreateEntity()) {}
        Entity(Handle&& handle) : handle(handle) {}

        // If no scene is specified use World
        Entity(EntityID id) : handle(World.ents, id) {}

        operator Handle() const { return handle; }
        operator EntityID() const { return handle.entity(); }

        operator bool() const { return handle.entity() != EntityNull && handle.valid(); }

        bool operator==(const Entity& that) const { return handle == that.handle; }
        bool operator==(const EntityID& that) const { return handle == that; }
        
        Scene& GetScene() const {
            return *handle.registry()->ctx().at<Scene*>();
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