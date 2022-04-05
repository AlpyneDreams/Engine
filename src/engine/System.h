#pragma once

#include <vector>
#include <memory>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

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

        // TODO: multiple systems?

        std::map<std::type_index, std::unique_ptr<System>> systems;

    public:
    
        template <SystemClass Sys>
        Sys& GetSystem() const {
            // TODO: ew
            auto ptr = systems.at(typeid(Sys)).get();
            return *static_cast<Sys*>(ptr);
        }

        template <SystemClass Sys>
        Sys& AddSystem(auto&... args) {
            if (!systems.contains(typeid(Sys))) {
                systems[typeid(Sys)] = std::make_unique<Sys>(args...);
                
                // If Start() has already been called, then call
                // it on new systems as soon as they're created.
                if (started) {
                    systems[typeid(Sys)]->Start();
                }
            }
            // TODO: This is redundant
            return GetSystem<Sys>();
        }

        template <SystemClass Sys>
        void RemoveSystem() {
            systems.erase(typeid(Sys));
        }

    // Iteration:

        auto begin() const {return systems.begin();}
        auto end() const {return systems.end();}
    
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