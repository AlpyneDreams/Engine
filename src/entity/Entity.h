#pragma once

#include <concepts>

#include "common/Traits.h"

#include "Common.h"
#include "Scene.h"
#include "Component.h"

namespace engine
{
    /**
     * Entity is a handle for a scene entity.
     *   - It consists of a scene registry and an ID.
     *   - It is a container for components, which
     *     provide both logic and data.
     */
    struct Entity
    {
        // Handle: registry ptr + entity ID
        Handle handle;

        // Create a new entity
        Entity() : handle(World.CreateEntity()) {}

        // Grab an entity by handle or scene & ID
        Entity(Handle&& handle) : handle(handle) {}
        Entity(Scene& scene, EntityID id) : handle(scene.ents, id) {}

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

        // Delete an entity, (handle will become null)
        void Delete() {
            handle.registry()->destroy(handle.entity());
            handle = Handle(World.ents, EntityNull);
        }

        // Component Management //

        template <class C>
        bool HasComponent() {
            return handle.any_of<C>();
        }

        template <class C>
        C& AddComponent() {
            // Add required components, if any
            if constexpr (derived_from_template<C, RequireComponents>) {
                C::AddRequiredComponents(handle);
            }

            // Create component instance (get if it already exists)
            C& component = handle.get_or_emplace<C>();
            
            // If this component is an entity, then share our handle with it
            if constexpr (std::derived_from<C, Entity>) {
                static_cast<Entity&>(component).handle = handle;
            }
            
            if constexpr (std::derived_from<C, Behavior>) {

            }
            
            return component;
        }

        template <class C>
        C& GetComponent() const {
            return handle.get<C>();
        }

        template <class C>
        C& GetOrAddComponent()
        {
            if (!HasComponent<C>()) {
                return AddComponent<C>();
            }
            return GetComponent<C>();
        }

        template <class C>
        void RemoveComponent() {
            if constexpr (std::derived_from<C, Behavior>) {

            }
            // remove<C>(): the component does not need to exist
            handle.remove<C>();
        }

        void RemoveComponent(ComponentID id) {
            handle.registry()->storage(id)->second.remove(handle.entity());
        }
    };

    // A pure Entity cannot be a component.
    template<> Entity& Entity::AddComponent<Entity>() = delete;
    template<> Entity& Entity::GetComponent<Entity>() const = delete;
    template<> void Entity::RemoveComponent<Entity>() = delete;
}