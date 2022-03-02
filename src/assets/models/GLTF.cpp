#include "GLTF.h"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <string>
#include <tiny_gltf.h>

using namespace tinygltf;
static TinyGLTF GLTF;

namespace engine
{
    void LoadModel(const char* path)
    {
        Model model;
        std::string err, warn;

        bool success = GLTF.LoadASCIIFromFile(&model, &err, &warn, path);
            
        if (!warn.empty()) {
            printf("[GLTF] Warning: %s\n", warn);
        }

        if (!err.empty()) {
            printf("[GLTF] Error: %s\n", err);
        }

        if (!success) {
            printf("[GLTF] Failed to load GLTF model: \"%s\"\n", path);
        }

        
    }
}