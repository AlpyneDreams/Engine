#pragma once

#include "common/Common.h"
#include "engine/System.h"
#include "editor/Editor.h"
#include "entity/components/Transform.h"
#include "math/Math.h"
#include "console/ConVar.h"
#include "input/Input.h"
#include "platform/Cursor.h"

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

            if (Mouse.GetButtonDown(Mouse::Right) || Keyboard.GetKeyDown(Key::Z)) {
                Cursor.SetMode(Cursor::Locked);
                Cursor.SetVisible(false);
            }

            if (Mouse.GetButton(Mouse::Right) || Keyboard.GetKey(Key::Z))
            {
                Transform& transform = Editor.editorCamera.GetComponent<Transform>();
                vec3 euler = transform.GetEulerAngles();
                int2 mouse = Mouse.GetMotion() / int2(2);
                transform.SetEulerAngles(vec3(euler.x + mouse.y, euler.y + mouse.x, euler.z));
            }

            else if (Mouse.GetButtonUp(Mouse::Right) || Keyboard.GetKeyUp(Key::Z)) {
                Cursor.SetMode(Cursor::Normal);
                Cursor.SetVisible(true);
            }
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
