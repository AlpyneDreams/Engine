
#include "Editor.h"

#include "engine/Engine.h"
#include "imgui/ConsoleWindow.h"
#include "editor/Keybinds.h"
#include "editor/gui/Layout.h"
#include "editor/gui/Outline.h"
#include "editor/gui/Inspector.h"
#include "editor/gui/SceneView.h"
#include "editor/gui/AssetBrowser.h"
#include "editor/Gizmos.h"

#include "entity/components/Transform.h"
#include "entity/components/Camera.h"
#include "render/TextureFormat.h"

#include <bit>


namespace engine::editor
{
    void Editor::Run()
    {
        Init();
        Loop();
        Shutdown();
    }

    void Editor::Init()
    {
        // Add engine systems...
        Engine.systems.AddSystem<editor::Keybinds>();
        Engine.systems.AddSystem<editor::Layout>();
        console       = &Engine.systems.AddSystem<GUI::ConsoleWindow>();
        outline       = &Engine.systems.AddSystem<editor::Outline>();
        inspector     = &Engine.systems.AddSystem<editor::Inspector>();
        sceneView     = &Engine.systems.AddSystem<editor::SceneView>();
        assetBrowser  = &Engine.systems.AddSystem<editor::AssetBrowser>();

        // Initialize engine
        Engine.Init();

        render::Render& r = Engine.Render;

        // Initialize gizmos
        Gizmos.Init();
        
        // Load editor shaders
        sh_Color  = r.LoadShader("vs_basic", "fs_color");
        sh_Grid   = r.LoadShader("vs_grid", "fs_grid");

        // Setup editor render targets
        auto [width, height] = Engine.window->GetSize();
        rt_SceneView = r.CreateRenderTarget(width, height);
        rt_ObjectID = r.CreateRenderTarget(width, height, render::TextureFormat::R32F, render::TextureFormat::None);
        rt_ObjectID->SetReadBack(true);
        
        // Setup editor camera
        editorCamera.SetName("Editor Camera");
        Camera& camera = editorCamera.AddComponent<Camera>();
        editorCamera.GetComponent<Transform>().position = vec3(2, 2.5, -3.5);
        editorCamera.GetComponent<Transform>().SetEulerAngles(vec3(30, -30, 0));
        camera.renderTarget = rt_SceneView;

        // Setup Object ID pass
        Engine.renderSystem.OnEndCamera += [](render::RenderContext& ctx)
        {
            ctx.SetupCamera();
            ctx.r.SetRenderTarget(editor::Editor.rt_ObjectID);
            ctx.r.SetClearColor(true, Colors.Black);
            ctx.r.SetBlendFunc(render::BlendFuncs::Normal);
            ctx.DrawRenderersWith([&](EntityID id) {
                ctx.r.SetShader(editor::Editor.sh_Color);
                uint i = uint(id);
                float f = std::bit_cast<float>(i + 1); // add 1 as 0 is for background
                ctx.r.SetUniform("u_color", vec4(f, 0.f, 0.f, 1.0f));
            });
        };
    }

    void Editor::Loop()
    {
        Engine.Loop();
    }

    void Editor::Shutdown()
    {
        Engine.Shutdown();
    }

    void Editor::PickObject(uint2 mouse)
    {
        rt_ObjectID->ReadTexture([=](float* data, size_t size, size_t width)
        {
            for (float* ptr = data; ptr < data + size;)
            {
                uint x = (ptr - data) % width;
                uint y = (ptr - data) / width;
                uint id = std::bit_cast<uint>(*ptr++);

                if (mouse.x == x && mouse.y == y)
                {
                    if (id == 0)
                        Selection.Clear();
                    else
                        // subtract 1, because 0 is reserved for null
                        Selection.Select(EntityID(id - 1));
                }
            }
        });
    }

}