#pragma once

#include "console/Console.h"
#include "common/String.h"
#include "common/Filesystem.h"
#include "core/Mesh.h"

namespace engine
{
    // TODO: Better way to define asset loaders.

    // Override this to support different asset formats
    template <class Asset, FixedString Ext>
    Asset* ImportAsset(const fs::Path& path);

    inline struct Assets
    {
        using Path = fs::Path;
        std::vector<Path> searchPaths;

        Assets()
        {
            // TODO: Load search paths from app info file
            AddSearchPath("core");
        }

    // Asset Loading //

        template <class T, FixedString Ext>
        T* Load(const char* path)
        {
            return ImportAsset<T, Ext>(FindFile(path));
        }

        // TODO: Dynamic extension
        template <class T>
        T* Load(const char* path) {
            return nullptr;
        }

        // TODO: Map ext -> type
        void* Load(const char* path) {
            return nullptr;
        }
        
    // Search Paths //

        void AddSearchPath(const char* path)
        {
            Path dir = Path(path);
            if (!fs::exists(dir)) {
                Console.Error("[Assets] Failed to find search path '{}'", path);
                return;
            }
            searchPaths.push_back(dir);
        }

        // Find actual path of asset in search paths
        Path FindFile(const char* path) const
        {
            for (const Path& dir : searchPaths)
            {
                Path fullPath = dir / path;
                if (fs::exists(fullPath))
                    return fullPath;
            }

            Console.Error("[Assets] Can't find file: '{}'", path);
            return Path();
        }

    } Assets;
}
