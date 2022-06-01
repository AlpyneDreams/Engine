#pragma once

#include "Window.h"
#include <imgui.h>
#include <iostream>

#include "console/Console.h"
#include "console/ConCommand.h"
#include "console/ConVar.h"

#include "input/Input.h"

namespace engine::GUI
{
    struct ConsoleWindow : public Window
    {
        ConsoleWindow() : Window("Console", 512, 512, false) {}

        bool scrollToBottom = false;

        void Update() final override
        {
            if (Input.GetKeyUp(Key::Grave)) {
                open = !open;
            }
            Window::Update();
        }

        // Based on imgui_demo.cpp ExampleAppConsole
        void Draw() final override
        {
            ImGui::PushFont(GUI::FontMonospace);

            // Reserve enough left-over height for 1 separator + 1 input text
            const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
            
            // Print each log line
            for (auto& str : Console.log) {
                if (strncmp(str.c_str(), "> ", 2) == 0)
                    ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.6f, 1.0f), "%s", str.c_str());
                else
                    ImGui::TextUnformatted(str.c_str());
            }

            // Auto-scroll to bottom
            if (scrollToBottom || ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);
            scrollToBottom = false;

            ImGui::EndChild();
            ImGui::Separator();

            char command[1024] = "";
            if (ImGui::InputText("Command", command, 1024, ImGuiInputTextFlags_EnterReturnsTrue)) {
                ImGui::SetKeyboardFocusHere(-1);
                scrollToBottom = true;
                Console.Execute(command);
            }

            ImGui::PopFont();
        }
    };
}