#pragma once

#include "Entity.h"
#include "Component.h"
#include "engine/System.h"

namespace engine
{
    struct Behavior : Entity, Component, System
    {
        virtual void Start() override {}
        virtual void Update() override {}
        virtual void Tick() override {}
    };
}