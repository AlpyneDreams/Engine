#pragma once


// TODO: Replace this with a better, simpler, parser
#include "hammer/deps/vdf_parser.h"

#include "console/Console.h"

#include <map>
#include <memory>
#include <string>
#include <utility>

namespace engine::hammer
{
    struct KeyValues
    {
    private:
        using KVObject = std::map<std::string, std::shared_ptr<KeyValues>>;
    public:
        enum Type { Null, String, Object };
        Type type = Type::Object;

        std::string name;

    private:
        // TODO: union or variant
        std::string value;
        
        KVObject object;
        std::shared_ptr<KeyValues> next = nullptr;

        static KeyValues null;
    public:

        KeyValues() {}
        KeyValues(Type type) : type(type) {}
        explicit KeyValues(std::string name, std::string value) : type(String), name(name), value(value) {}

        operator std::string() const { return type == Object ? "[object Object]" : value; }
        operator int() const { return std::stoi(*this); }
        operator bool() const { return std::stoi(*this) != 0; }

        bool operator ==(const KeyValues& kv) const
        {
            if (type != kv.type)
                return false;
            switch (type)
            {
                case String:
                    return value == kv.value;
                case Object:
                    return false; // TODO
                default:
                    return true;
            }
        }

        bool operator !=(const KeyValues& kv) const { return !(*this == kv); }

        static KeyValues Parse(std::string& str)
        {
            return tyti::vdf::read<KeyValues>(str.begin(), str.end());
        }

        KeyValues& Get(const char* key) {
            return object.contains(key) ? *object[key] : null;
        }
        
        KeyValues& operator [](const char* key) {
            return Get(key);
        }

    // Iterate all child keys. //

        struct Iterator
        {
            KVObject::iterator it;
            Iterator(KVObject::iterator it) : it(it) {}
            auto& operator *() { return *it->second; }
            auto& operator ++() { return ++it; }
            bool operator ==(const Iterator& that) const { return it == that.it; }
        };

        auto begin() { return Iterator(object.begin()); }
        auto end() { return Iterator(object.end()); }

    // Iterate multiple siblings with the same key. //

        struct Iterable
        {
            struct Iterator
            {
                KeyValues* kv;
                Iterator(KeyValues* ptr) : kv(ptr) {}
                auto& operator *() { return *kv; }
                auto& operator ++()
                {
                    if (kv)
                        kv = kv->next.get();
                    return *this;
                }
                bool operator ==(const Iterator& that) const { return kv == that.kv; }
            };

            KeyValues* kv;
            Iterable(KeyValues* ptr) : kv(ptr) {}
            auto begin() { return Iterator(kv); }
            auto end() { return Iterator(nullptr); }
        };

        Iterable each(const char* key) {
            return Iterable(&Get(key));
        }

        Iterable Each(const char* key) { return each(key); }

        //Iterable operator ()(const char* key) { return each(key); }

    // tyti::vdf API //

        void add_attribute(std::string key, std::string value)
        {
            object.emplace(key, std::make_unique<KeyValues>(key, value));
        }
        
        void add_child(std::unique_ptr<KeyValues> child)
        {
            std::shared_ptr<KeyValues> obj { child.release() };
            if (!object.contains(obj->name))
            {
                object.emplace(obj->name, obj);
                return;
            }
            else
            {
                auto kv = object[obj->name];
                while (kv->next)
                    kv = kv->next;
                kv->next = obj;
            }
        }

        void set_name(std::string nombre) {
            name = nombre;
        }

    };

    inline KeyValues KeyValues::null = KeyValues::Null;

}

// FMT/ostream support

namespace engine::hammer
{
    inline std::ostream& operator<<(std::ostream& os, const KeyValues& kv) {
        return os << kv.operator std::string();
    }
}
template <> struct fmt::formatter<engine::hammer::KeyValues> : ostream_formatter {};


// All the C++ boilerplate necessary to allow structured binding:
//  auto& [key, value] = kv;

namespace std
{
    template <>
    struct tuple_size<engine::hammer::KeyValues> {
        static constexpr size_t value = 2;
    };
    
    template<>
    struct tuple_element<0, engine::hammer::KeyValues> {
        using type = std::string;
    };

    template<>
    struct tuple_element<1, engine::hammer::KeyValues> {
        using type = engine::hammer::KeyValues&;
    };
}

namespace engine::hammer
{
    template<std::size_t Index>
    std::tuple_element_t<Index, engine::hammer::KeyValues>& get(engine::hammer::KeyValues& kv)
    {
        if constexpr (Index == 0) return kv.name;
        if constexpr (Index == 1) return kv;
    }
}
