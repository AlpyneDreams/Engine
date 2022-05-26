#pragma once

#include <typeinfo>
#include <typeindex>
#include <string>
#include <vector>
#include <cstddef>

namespace engine
{
    /** Extend this class to enable RTTI generation for subclasses. */
    struct Reflect {};
};

namespace engine::reflect
{
    struct Variable
    {
        const char* name;
        std::type_index type;
        size_t offset;
    };

    struct Class
    {
        std::vector<Variable> fields;
    };
};

// This namespace contains all autogenerated RTTI data.
namespace engine::rtti
{
    using namespace engine::reflect;

    template <class T>
    Class RTTI;
};