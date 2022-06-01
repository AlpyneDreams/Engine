
#include "Editor.h"

#include "engine/Engine.h"
#include "imgui/ConsoleWindow.h"
#include "editor/gui/Outline.h"
#include "editor/gui/Inspector.h"


namespace engine::editor
{
    void Editor::Run()
    {
        Engine.systems.AddSystem<GUI::ConsoleWindow>();
        Engine.systems.AddSystem<editor::Outline>();
        Engine.systems.AddSystem<editor::Inspector>();

        Engine.Init();
        Engine.Loop();
        Engine.Shutdown();
    }
}