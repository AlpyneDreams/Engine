#pragma once

#include "common/Common.h"
#include "engine/System.h"
#include "editor/Editor.h"
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

                CoordinateSpacePicker();

                if (ImGui::BeginMenu("Window"))
                {
                    ImGui::MenuItem("Console", "", &Editor.console->open);
                    ImGui::MenuItem("Outline", "", &Editor.outline->open);
                    ImGui::MenuItem("Inspector", "", &Editor.inspector->open);
                    ImGui::MenuItem("GUI Demo", "", &gui_demo.value);
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);
        }

        void CoordinateSpacePicker()
        {
            const char* items[] = {
                ICON_MC_WEB " World",
                ICON_MC_CUBE_OUTLINE " Local"
            };
            int i = int(space);
            auto label = std::string(items[i]) + " " ICON_MC_MENU_DOWN;
            if (ImGui::BeginMenu(label.c_str())) {
                for (int j = 0; j < std::size(items); j++) {
                    if (ImGui::MenuItem(items[j])) {
                        space = Space(j);
                    }
                }
                ImGui::EndMenu();
            }
        }
    };
}
