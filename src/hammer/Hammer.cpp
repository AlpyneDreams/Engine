
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
#include "hammer/gui/Keybinds.h"

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

        KeyValues kv = KeyValues::Parse(vmf);

        map = VMF(kv);
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
        shader = Engine.Render.LoadShader("hammer_flat");
    }

    void MapRender::Update()
    {
        r.SetRenderTarget(Editor.rt_SceneView);
        r.SetShader(shader);
        
        DrawEntity(Hammer.map.world);

        for (auto& ent : Hammer.map.entities)
            DrawEntity(ent);

        editor::Gizmos.DrawIcon(vec3(0, 1, 0), editor::Gizmos.icnLight);
    }

    inline void MapRender::DrawEntity(MapEntity &entity)
    {
        for (auto& solid : entity.solids)
        {
            r.SetUniform("u_color", solid.editor.color);
            r.SetTransform(glm::identity<mat4x4>());
            r.DrawMesh(&solid.mesh);
        }
    }
}

int main(int argc, char* argv[])
{
    using namespace engine::hammer;
    Hammer.Run();
}
