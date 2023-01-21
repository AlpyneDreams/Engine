
#include "Tools.h"
#include "engine/Engine.h"
#include "editor/Gizmos.h"
#include "editor/Selection.h"

#include <bit>

namespace engine::editor
{
    void Tools::Init()
    {
        console = &Engine.systems.AddSystem<GUI::ConsoleWindow>();

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
        editorCamera.transform.position = vec3(2, 2.5, -3.5);
        editorCamera.transform.SetEulerAngles(vec3(30, -30, 0));
        editorCamera.camera.renderTarget = rt_SceneView;
        
        // Setup camera renderer
        Engine.renderSystem.OnBeginFrame += [](render::Render& r) {
            Engine.renderSystem.DrawCamera(editor::Tools.editorCamera.camera, editor::Tools.editorCamera.transform);
        };

        // Setup Object ID pass
        Engine.renderSystem.OnEndCamera += [](render::RenderContext& ctx)
        {
            ctx.SetupCamera();
            ctx.r.SetRenderTarget(editor::Tools.rt_ObjectID);
            ctx.r.SetClearColor(true, Colors.Black);
            ctx.r.SetBlendFunc(render::BlendFuncs::Normal);
            ctx.DrawRenderersWith([&](EntityID id) {
                ctx.r.SetShader(editor::Tools.sh_Color);
                uint i = uint(id);
                float f = std::bit_cast<float>(i + 1); // add 1 as 0 is for background
                ctx.r.SetUniform("u_color", vec4(f, 0.f, 0.f, 1.0f));
            });
        };
    }

    void Tools::Loop()
    {
        Engine.Loop();
    }

    void Tools::Shutdown()
    {
        Engine.Shutdown();
    }

    void Tools::PickObject(uint2 mouse)
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
                        Selection.Select(id - 1);
                }
            }
        });
    }

}