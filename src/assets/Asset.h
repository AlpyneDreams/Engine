#pragma once

#include "console/Console.h"
#include "common/String.h"
#include "common/Filesystem.h"
#include "core/Mesh.h"

namespace engine
{
    // TODO: Better way to load assets.

    // Call this to load an asset
    template <class Asset, FixedString Ext>
    Asset* LoadAsset(const char* path, const char* dir = "core");

    // Override this to support different asset formats
    template <class Asset, FixedString Ext>
    Asset* ImportAsset(const char* path);

    template <>
    Mesh* ImportAsset<Mesh, ".OBJ">(const char* path);


    template <class Asset, FixedString Ext>
    inline Asset* LoadAsset(const char* path, const char* dir)
    {
        // TODO: std::filesystem::path
        std::string fullPath = std::string(dir) + "/" + path;
        if (!fs::exists(fullPath)) {
            Console.Error("[Assets] Cannot find asset: '{}'", path);
        }

        return ImportAsset<Asset, Ext>(fullPath.c_str());
    }
}
