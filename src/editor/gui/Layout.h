#pragma once

#include "common/Common.h"
#include "core/Primitives.h"
#include "engine/System.h"
#include "editor/Editor.h"
#include "entity/components/Transform.h"
#include "entity/components/MeshRenderer.h"
#include "math/Math.h"
#include "console/ConVar.h"

#include "imgui/Common.h"
#include <imgui.h>

#include "imgui/IconsMaterialCommunity.h"

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
                    ImGui::MenuItem("Console", "", &Editor.console->open);
                    ImGui::MenuItem("Scene", "", &Editor.sceneView->open);
                    ImGui::MenuItem("Outline", "", &Editor.outline->open);
                    ImGui::MenuItem("Inspector", "", &Editor.inspector->open);
                    ImGui::MenuItem("GUI Demo", "", &gui_demo.value);
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            // ImGuiDockNodeFlags_PassthruCentralNode
            ImGui::DockSpaceOverViewport(NULL);
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
        }
    };
}
