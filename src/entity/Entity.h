#pragma once

#include "common/Common.h"
#include "math/Math.h"

// Subject to change.
#define ENTT_ID_TYPE    std::uint32_t
#include <entt/entt.hpp>
#include <entt/meta/ctx.hpp>

#include <set>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "engine/System.h"

namespace engine
{
    struct Component {};

    using EntityID = entt::entity;
    using Handle = entt::handle;    // TODO: Rename or move into a class?

    // TODO: Scene.h

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
            return handle.emplace<C>();
        }

        template <class C>
        C& GetComponent() {
            return handle.get<C>();
        }

        template <class C>
        void RemoveComponent() {
            handle.erase<C>();
        }
    };

    // TOOD: Behvaior.h

    struct Behavior : Entity, Component, System
    {
        Behavior(Handle& handle) : Entity(handle) {}

        virtual void Start() override {}
        virtual void Update() override {}
        virtual void Tick() override {}
    };

    template <>
    Behavior& Entity::AddComponent<Behavior>() {
        return handle.emplace<Behavior>(handle);
    }

    // A pure Entity cannot be a component.
    template<> Entity& Entity::AddComponent<Entity>() = delete;
    template<> Entity& Entity::GetComponent<Entity>() = delete;
    template<> void Entity::RemoveComponent<Entity>() = delete;

    // TODO: Some example components. Move out of here.

    struct Transform : Component
    {
        Vector3     position;
        Quaternion  rotation;
        Vector3     scale;
    };

    struct Parent : Component
    {
        Entity* parent;
    };

}