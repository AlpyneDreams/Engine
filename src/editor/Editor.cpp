
#include "Editor.h"

#include "engine/Engine.h"
#include "imgui/ConsoleWindow.h"
#include "editor/gui/Layout.h"
#include "editor/gui/Outline.h"
#include "editor/gui/Inspector.h"
#include "editor/gui/SceneView.h"

#include "entity/components/Transform.h"
#include "entity/components/Camera.h"


namespace engine::editor
{
    void Editor::Run()
    {
        Engine.systems.AddSystem<editor::Layout>();
        console   = &Engine.systems.AddSystem<GUI::ConsoleWindow>();
        outline   = &Engine.systems.AddSystem<editor::Outline>();
        inspector = &Engine.systems.AddSystem<editor::Inspector>();
        sceneView = &Engine.systems.AddSystem<editor::SceneView>();

        editorCamera.SetName("Editor Camera");
        Camera& camera = editorCamera.AddComponent<Camera>();
        editorCamera.GetComponent<Transform>().position = vec3(0, 0, -35);

        Engine.Init();

        render::Render& r = Engine.Render;

        auto [width, height] = Engine.window->GetSize();
        rt_SceneView = r.CreateRenderTarget(uint(width), uint(height));

        camera.renderTarget = rt_SceneView;

        Engine.Loop();
        Engine.Shutdown();
    }
}