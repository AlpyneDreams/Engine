#pragma once

#include <vector>
#include <memory>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include "common/Ranges.h"

// TODO: Move this into /entity/? or keep in /engine/? 

namespace engine
{
    struct System
    {
        virtual void Start() {}
        virtual void Update() {}
        virtual void Tick() {}

        virtual ~System() {}
    };

    struct Behavior;

    // TODO: Keep this trait?
    template <class T>
    concept SystemClass = std::is_base_of_v<System, T> && !std::is_same_v<Behavior, T>;

    // TODO
    template <class... Components>
    struct ComponentSystem;

    // e.g.
    // struct TransformSystem : ComponentSystem<Transform>

    struct SystemGroup : System
    {
    private:
        bool started = false;

        // TODO: non-owned systems?

        std::multimap<std::type_index, std::shared_ptr<System>> systems;

    public:
        // Can be destructured to a std::pair<std::type_index, std::shared_ptr<System>>
        using Iterator   = decltype(systems)::const_iterator;
        using SystemList = IteratorRange<Iterator>;
    
        template <SystemClass Sys>
        SystemList GetSystems() const {
            return systems.equal_range(typeid(Sys));
        }

        template <SystemClass Sys>
        Sys& AddSystem(auto&... args)
        {
            auto& [type, system] = *systems.insert({ typeid(Sys), std::make_shared<Sys>(args...) });
                
            // If Start() has already been called, then call
            // it on new systems as soon as they're created.
            if (started) {
                system->Start();
            }

            return *system.get();
        }

        // Remove all systems of type Sys
        template <SystemClass Sys>
        void RemoveSystems() {
            systems.erase(typeid(Sys));
        }

        // Remove all systems of 'type'
        void RemoveSystems(std::type_index type) {
            systems.erase(type);
        }

        // Removes a single system
        void RemoveSystem(Iterator iterator) {
            systems.erase(iterator);
        }

    // Iteration:

        Iterator begin() const {return systems.begin();}
        Iterator end() const {return systems.end();}
    
    // System overrides:

        void Start() final override {
            started = true;
            for (auto& [t, sys] : systems) {sys->Start();}
        }

        void Update() final override {
            for (auto& [t, sys] : systems) {sys->Update();}
        }

        void Tick() final override {
            for (auto& [t, sys] : systems) {sys->Tick();}
        }
    };
}