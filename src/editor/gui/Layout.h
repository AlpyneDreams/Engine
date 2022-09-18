#pragma once

#include "common/Common.h"
#include "core/Primitives.h"
#include "engine/System.h"
#include "editor/Editor.h"
#include "entity/components/Transform.h"
#include "entity/components/MeshRenderer.h"
#include "entity/components/Camera.h"
#include "math/Math.h"
#include "console/ConVar.h"

#include "imgui/Common.h"
#include <imgui.h>
#include <imgui_internal.h>

#include "imgui/IconsMaterialCommunity.h"

#include "editor/Icons.h"

namespace engine {
    extern ConVar<bool> gui_demo;
}

namespace engine::editor
{
    struct Layout : System
    {
        Space space = Space::World;

        void Start() override
        {
            ImGuiIO& io = ImGui::GetIO();
            io.IniFilename = "editor.ui.ini";
        }

        void Update() override
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Quit")) {
                        Console.Execute("quit");
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Window"))
                {
                    ImGui::MenuItem(Editor.console->name.c_str(), "`", &Editor.console->open);
                    ImGui::MenuItem(Editor.sceneView->name.c_str(), "", &Editor.sceneView->open);
                    ImGui::MenuItem(Editor.outline->name.c_str(), "", &Editor.outline->open);
                    ImGui::MenuItem(Editor.inspector->name.c_str(), "", &Editor.inspector->open);
                    ImGui::MenuItem(Editor.assetBrowser->name.c_str(), "Ctrl+Space", &Editor.assetBrowser->open);
                    ImGui::MenuItem(ICON_MC_APPLICATION_OUTLINE " GUI Demo", "", &gui_demo.value);
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            if (ImGui::BeginViewportSideBar("BottomBar", ImGui::GetMainViewport(), ImGuiDir_Down, 20.0f, ImGuiWindowFlags_MenuBar))
            {
                if (ImGui::BeginMenuBar())
                {
                    WindowToggleButton(Editor.assetBrowser, 64.0f, "Ctrl+Space");
                    WindowToggleButton(Editor.console, 72.0f, "`");
                    ImGui::EndMenuBar();
                }
                ImGui::End();
            }

            // ImGuiDockNodeFlags_PassthruCentralNode
            ImGui::DockSpaceOverViewport(NULL);
        }

        static void WindowToggleButton(GUI::Window* window, float width = 64.0f, const char* tooltip = nullptr)
        {
            if (ImGui::Selectable(window->name.c_str(), window->visible, ImGuiSelectableFlags_None, ImVec2(width, 20.0f)))
            {
                window->ToggleOrFocus();
            }
            if (tooltip && ImGui::IsItemHovered())
                ImGui::SetTooltip("%s", tooltip);
        }

        static void AddObjectMenu()
        {
            if (ImGui::MenuItem(ICON_MC_AXIS_ARROW " Entity")) {
                Entity ent = Editor.AddEntity();
                ent.AddComponent<Transform>();
            }
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_MC_CUBE_OUTLINE " Cube")) {
                Entity ent = Editor.AddEntity();
                ent.SetName("Cube");
                ent.AddComponent<Transform>();
                ent.GetOrAddComponent<MeshRenderer>().mesh = &Primitives.Cube;
            }
            if (ImGui::MenuItem(ICON_MC_KETTLE_OUTLINE " Teapot")) {
                Entity ent = Editor.AddEntity();
                ent.SetName("Teapot");
                ent.AddComponent<Transform>();
                ent.GetOrAddComponent<MeshRenderer>().mesh = &Primitives.Teapot;
            }
            if (ImGui::MenuItem(ICON_MC_SQUARE_OUTLINE " Plane")) {
                Entity ent = Editor.AddEntity();
                ent.SetName("Plane");
                ent.AddComponent<Transform>();
                ent.GetOrAddComponent<MeshRenderer>().mesh = &Primitives.Plane;
            }
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_MC_VIDEO " Camera")) {
                Entity ent = Editor.AddEntity();
                ent.SetName("Camera");
                ent.AddComponent<Transform>();
                ent.AddComponent<Camera>();
            }
        }
    };
}
