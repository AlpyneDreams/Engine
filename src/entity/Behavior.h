#pragma once

#include "Entity.h"
#include "Component.h"
#include "engine/System.h"
#include "entity/Common.h"

namespace engine
{
    /**
     * Behavior is a Component that provides per-entity
     * System logic and can access its Entity's data.
     *
     * It is most similar to MonoBehaviour in Unity.
     */
    struct Behavior : Entity, Component, System
    {
        // Handle will be updated by AddComponent
        Behavior() : Entity(EntityNull) {}

        virtual void Start() override {}
        virtual void Update() override {}
        virtual void Tick() override {}
    };
}