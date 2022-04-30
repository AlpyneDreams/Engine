#pragma once

#include <vector>
#include <memory>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <type_traits>

#include "common/Ranges.h"
#include "common/VTable.h"

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

    using SystemFunc = void(System*);

    template <>
    struct VTable<System>
    {
        SystemFunc* Start;
        SystemFunc* Update;
        SystemFunc* Tick;
        void*       Destructor;
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
        // TODO: system update order
        // TODO: VTable checks for Behaviors

        std::multimap<std::type_index, std::shared_ptr<System>> systems;
        std::map<System*, SystemFunc*> OnStart, OnUpdate, OnTick;

    public:
        // Can be destructured to a std::pair<std::type_index, std::shared_ptr<System>>
        using Iterator   = decltype(systems)::const_iterator;
        using SystemList = IteratorRange<Iterator>;

        SystemGroup() {}
        SystemGroup(auto*... sys) {
            (systems.insert({typeid(decltype(sys)), std::shared_ptr<System>(sys)}), ...);
        }
    
        template <SystemClass Sys>
        Sys& AddSystem(auto&... args)
        {
            static System nullsystem;
            static VTable<System>& base = GetVTable(&nullsystem);

            auto system = std::make_shared<Sys>(args...);
            auto* sys   = system.get();

            systems.insert({typeid(Sys), system});

            VTable<System>& vt = GetVTable<System>(sys);

            if (vt.Start != base.Start) {
                OnStart.insert({sys, vt.Start});

                // If Start() has already been called, then call
                // it on new systems as soon as they're created.
                if (started) {
                    system->Start();
                }
            }

            if (vt.Update != base.Update) {
                OnUpdate.insert({sys, vt.Update});
            }

            if (vt.Tick != base.Tick) {
                OnTick.insert({sys, vt.Tick});
            }

            return *sys;
        }

        template <SystemClass Sys>
        SystemList GetSystems() const {
            return GetSystems(typeid(Sys));
        }

        SystemList GetSystems(std::type_index type) const {
            return systems.equal_range(type);
        }

        // Remove all systems of type Sys
        template <SystemClass Sys>
        void RemoveSystems() {
            RemoveSystems(typeid(Sys));
        }

        // Remove all systems of 'type'
        void RemoveSystems(std::type_index type) {
            for (auto& [t, sys] : GetSystems(type)) {
                auto* ptr = sys.get();
                UnregisterCallbacks(ptr);
            }
            systems.erase(type);
        }

        // Removes a single system
        void RemoveSystem(Iterator iterator) {
            systems.erase(iterator);
            UnregisterCallbacks(iterator->second.get());
        }

    // Iteration:

        Iterator begin() const {return systems.begin();}
        Iterator end() const {return systems.end();}
    
    // System overrides:

        void Start() final override { started = true; Call(OnStart); }
        void Update() final override { Call(OnUpdate); }
        void Tick() final override { Call(OnTick); }

    protected:
        inline void Call(auto& event) {
            for (auto& [sys, Func] : event) { Func(sys); }
        }

        inline void UnregisterCallbacks(System* ptr)
        {
            OnStart.erase(ptr);
            OnUpdate.erase(ptr);
            OnTick.erase(ptr);
        }
    };
}