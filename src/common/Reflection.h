#pragma once

#include <typeinfo>
#include <typeindex>
#include <string>
#include <vector>
#include <cstddef>

#include <entt/core/type_info.hpp>

namespace engine
{
    /** Extend this class to enable RTTI generation for subclasses. */
    struct Reflect {};
};

namespace engine::reflect
{
    using Type = entt::type_info;
    
    // Get type info.
    // Has to be function not variable or else we can get invalid results sometimes
    template <typename T>
    Type TypeID() { return entt::type_id<T>(); }

    // Type hashes (constexpr)
    template <typename T>
    constexpr size_t TypeHash = entt::type_hash<T>::value();

    struct Field
    {
        const char* name;
        const char* displayName;
        Type type;
        size_t offset;

        template <typename T>
        T* GetPointer(void* obj) const {
            return reinterpret_cast<T*>((char*)obj + offset);
        }

        template <typename T>
        T& Get(void* obj) const {
            return *GetPointer<T>(obj);
        }
    };

    struct Class
    {
        const char* name;
        const char* displayName;
        size_t size;
        std::vector<Field> fields;
    };
};

// This namespace contains all autogenerated RTTI data.
namespace engine::rtti
{
    using namespace engine::reflect;

    template <class T>
    Class RTTI;
};