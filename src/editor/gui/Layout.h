#pragma once

#include "common/Common.h"
#include "engine/System.h"
#include "editor/Editor.h"

#include "imgui/Common.h"
#include <imgui.h>

namespace engine::editor
{
    struct Layout : System
    {
        void Start()
        {
            ImGuiIO& io = ImGui::GetIO();
            io.IniFilename = "editor.ui.ini";
        }

        void Update() override
        {
            if (ImGui::BeginMainMenuBar())
            {
                ImGui::EndMainMenuBar();
            }

            ImGui::DockSpaceOverViewport(NULL, ImGuiDockNodeFlags_PassthruCentralNode);
        }
    };
}
