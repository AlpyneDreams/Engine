
#include "hammer/Hammer.h"
#include "common/String.h"
#include "core/Primitives.h"
#include "core/VertexLayout.h"
#include "editor/Gizmos.h"
#include "hammer/KeyValues.h"

#include "console/Console.h"
#include "engine/Engine.h"
#include "imgui/ConsoleWindow.h"
#include "hammer/gui/Layout.h"
#include "hammer/gui/Viewport.h"
#include "hammer/Keybinds.h"

#include "common/Filesystem.h"

#include "math/Math.h"
#include <glm/gtx/normal.hpp>

#include <cstring>
#include <vector>


namespace engine::hammer
{
    static VertexLayout xyz = {
        VertexAttribute::For<float>(3, VertexAttribute::Position),
        VertexAttribute::For<float>(3, VertexAttribute::Normal, true),
    };

    void Hammer::Open(const char* path)
    {
        Console.Log("Open: '{}'", path);
        if (!fs::exists(path)) {
            Console.Error("Error: file '{}' does not exist", path);
            return;
        }
        
        std::string vmf = fs::readFile(path);

        // TODO: Use rain to fill this struct!

        KeyValues kv = KeyValues::Parse(vmf);
        auto& versioninfo = kv["versioninfo"];

        map = VMF();
        map.editorversion = versioninfo["editorversion"];
        map.editorbuild = versioninfo["editorbuild"];
        map.mapversion = versioninfo["mapversion"];
        map.formatversion = versioninfo["formatversion"];
        map.prefab = versioninfo["prefab"];

        // For each solid
        for (auto& solid : kv["world"].each("solid"))
        {
            Solid s = Solid { solid["id"] };

            std::vector<vec3>& vertices = *new std::vector<vec3>();
            std::vector<uint32>& indices = *new std::vector<uint32>();
            uint32 index = 0;

            // Add each side to mesh
            for (auto& side : solid.each("side"))
            {
                s.sides.push_back(Side {});
                
                auto points = str::split(side["plane"], ")");
                vec3 tri[3]; // bottom left, top left, top right

                // Parse points
                for (int i = 0; i < 3; i++)
                {
                    // TODO: Why can't we remove the '(' with str::trim
                    auto xyz = str::trim(points[i]);
                    xyz.remove_prefix(1);

                    auto coords = str::split(xyz, " ");
                    // Convert z-up to y-up
                    // TODO: Handedness?
                    float x = std::stof(std::string(coords[0]));
                    float y = std::stof(std::string(coords[2]));
                    float z = std::stof(std::string(coords[1]));
                    
                    // FIXME: Apply scale here for now
                    tri[i] = vec3(x, y, z) * vec3(0.0254f);
                }

                // Compute normal
                vec3 normal = glm::triangleNormal(tri[0], tri[1], tri[2]);
                
                // Add first triangle
                for (int i = 0; i < 3; i++)
                {
                    vertices.push_back(tri[i]);
                    vertices.push_back(normal);
                    indices.push_back(index++);
                }
                
                // Add second triangle
                indices.push_back(index - 3); // tri[0]
                indices.push_back(index - 1); // tri[2]

                vec3 dy = tri[1] - tri[0]; // top left - bottom left
                
                // Add 4th vertex
                vertices.push_back(tri[2] - dy);
                vertices.push_back(normal);
                indices.push_back(index++);
            }

            s.mesh = Mesh(xyz,
                &vertices[0].x, vertices.size() * sizeof(vec3),
                &indices[0], indices.size() * sizeof(uint32)
            );

            map.world.solids.push_back(s);
        }
    }

    void Hammer::Run()
    {
        Editor.Init();

        // Add hammer systems...
        Engine.systems.AddSystem<hammer::MapRender>();
        Engine.systems.AddSystem<hammer::Keybinds>();
        Engine.systems.AddSystem<hammer::Layout>();
        Editor.console = &Engine.systems.AddSystem<GUI::ConsoleWindow>();
        viewport = &Engine.systems.AddSystem<Viewport>();

        //Open("/home/alpyne/Desktop/test.vmf");
        Editor.Loop();
        Editor.Shutdown();
    }

    void MapRender::Start()
    {
        shader = Engine.Render.LoadShader("vs_cubes", "fs_cubes");
    }

    void MapRender::Update()
    {
        render::Render& r = Engine.Render;
        r.SetRenderTarget(Editor.rt_SceneView);
        r.SetShader(shader);
        
        for (auto& solid : Hammer.map.world.solids)
        {
            r.SetTransform(glm::identity<mat4x4>());
            r.DrawMesh(&solid.mesh);
        }

        editor::Gizmos.DrawIcon(vec3(0, 1, 0), editor::Gizmos.icnLight);
    }
}

int main(int argc, char* argv[])
{
    using namespace engine::hammer;
    Hammer.Run();
}
