#pragma once

#include "editor/Editor.h"
#include "entity/Common.h"
#include "entity/components/Transform.h"
#include "editor/Selection.h"
#include "entity/Scene.h"
#include "entity/Entity.h"
#include "entity/components/Camera.h"
#include "engine/Engine.h"
#include "input/Input.h"
#include "platform/Cursor.h"

#include "imgui/IconsMaterialCommunity.h"

namespace engine::editor
{
    struct SceneView : public GUI::Window
    {
        SceneView() : GUI::Window("Scene", 512, 512, true, ImGuiWindowFlags_MenuBar) {}

        Space space = Space::World;

        void PreDraw() override {
            // Set window padding to 0
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        }

        void PostDraw() override {
            ImGui::PopStyleVar();
        }

        void Draw() override
        {
            if (ImGui::BeginMenuBar())
            {
                CoordinateSpacePicker();
                ImGui::EndMenuBar();
            }

            // Return if window is collapsed
            if (!CheckResize()) {
                return;
            }

            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 size = ImGui::GetContentRegionAvail();
            ImVec2 max = ImVec2(pos.x + size.x, pos.y + size.y);

            if (ImGui::IsMouseHoveringRect(pos, max))
            {                
                // TODO: Make Z toggle instead of hold
                if (Mouse.GetButtonDown(Mouse::Right) || Keyboard.GetKeyDown(Key::Z))
                {
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
                else if (Mouse.GetButtonUp(Mouse::Right) || Keyboard.GetKeyUp(Key::Z))
                {
                    Cursor.SetMode(Cursor::Normal);
                    Cursor.SetVisible(true);
                }
            }

            // Copy from scene view render target into viewport
            ImGui::GetWindowDrawList()->AddImage(
                Editor.rt_SceneView->GetTexture(),
                pos, max,
                ImVec2(0, 0), ImVec2(1, 1)
            );
        }

        // Returns true if window is not collapsed
        bool CheckResize()
        {
            // If window resized: resize render target
            ImVec2 size = ImGui::GetContentRegionAvail();
            if (size.x != width || size.y != height)
            {
                // Window is collapsed or too small
                if (size.x == 0 || size.y == 0) {
                    return false;
                }

                width = uint(size.x);
                height = uint(size.y);

                // Resize framebuffer or viewport
                Editor.rt_SceneView->Resize(width, height);

                return true;
            }

            return true;
        }

        void CoordinateSpacePicker()
        {
            const char* items[] = {
                ICON_MC_WEB " World",
                ICON_MC_CUBE_OUTLINE " Local"
            };
            int i = int(space);
            auto label = std::string(items[i]) + " " ICON_MC_MENU_DOWN;
            PostDraw(); // revert 0 padding
            if (ImGui::BeginMenu(label.c_str())) {
                for (int j = 0; j < std::size(items); j++) {
                    if (ImGui::MenuItem(items[j])) {
                        space = Space(j);
                    }
                }
                ImGui::EndMenu();
            }
            PreDraw(); // reset padding to 0
        }

    };
}
