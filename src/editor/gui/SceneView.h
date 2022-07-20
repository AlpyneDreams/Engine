#pragma once

#include "editor/Editor.h"
#include "entity/Common.h"
#include "entity/components/MeshRenderer.h"
#include "entity/components/Transform.h"
#include "editor/Selection.h"
#include "entity/Scene.h"
#include "entity/Entity.h"
#include "entity/components/Camera.h"
#include "engine/Engine.h"
#include "input/Input.h"
#include "input/Keyboard.h"
#include "platform/Cursor.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>

#include "imgui/IconsMaterialCommunity.h"
#include "render/Render.h"

namespace engine::editor
{
    struct SceneView : public GUI::Window
    {
        SceneView() : GUI::Window(ICON_MC_IMAGE_SIZE_SELECT_ACTUAL " Scene", 512, 512, true, ImGuiWindowFlags_MenuBar) {}

        enum class Tool {
            Translate, Rotate, Scale, Universal
        };

        Tool activeTool = Tool::Translate;
        Space space = Space::World;
        Rect viewport;
        float cameraSpeed = 0.01f;

        void NoPadding() {
            // Set window padding to 0
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        }

        void ResetPadding() {
            ImGui::PopStyleVar();
        }

        void PreDraw() override {
            NoPadding();
        }

        static ImGuizmo::OPERATION GetOperation(Tool tool)
        {
            using enum ImGuizmo::OPERATION;
            switch (tool) {
                default:
                case Tool::Translate: return TRANSLATE;
                case Tool::Rotate:    return ROTATE;
                case Tool::Scale:     return SCALE;
                case Tool::Universal: return UNIVERSAL;
            }
        }

        void PostDraw() override
        {
            ResetPadding();
            
            // HACK: Set hovered window to NULL,
            // this fixes mouse over with ImGui docking
            ImGuiContext& g = *ImGui::GetCurrentContext();
            ImGuiWindow* hovered = g.HoveredWindow;
            g.HoveredWindow = NULL;

            ImGuizmo::BeginFrame();
            ImGuizmo::SetRect(viewport.x, viewport.y, viewport.w, viewport.h);

            Camera& camera = Editor.editorCamera.GetComponent<Camera>();
            //Transform& transform = Editor.editorCamera.GetComponent<Transform>();

            // Get camera matrices
            mat4x4 view = camera.ViewMatrix();
            mat4x4 proj = camera.ProjMatrix();
        
            
            Entity active = Selection.Active();
            if (active && active.HasComponent<Transform>())
            {
                Transform& transform = active.GetComponent<Transform>();
                float mtx[16];
                vec3 angles = transform.GetEulerAngles();
                ImGuizmo::RecomposeMatrixFromComponents(&transform.position[0], &angles[0], &transform.scale[0], mtx);

                ImGuizmo::MODE mode = space == Space::World ? ImGuizmo::MODE::WORLD : ImGuizmo::MODE::LOCAL;
                ImGuizmo::Manipulate(&view[0][0], &proj[0][0], GetOperation(activeTool), mode, &mtx[0]);
                
                ImGuizmo::DecomposeMatrixToComponents(mtx, &transform.position[0], &angles[0], &transform.scale[0]);
                transform.SetEulerAngles(angles);
            }

            //static mat4x4 grid = glm::identity<mat4x4>();
            //static mat4x4 cube = glm::identity<mat4x4>();
            //ImGuizmo::DrawGrid(&view[0][0], &proj[0][0], &grid[0][0], 100);
            //ImGuizmo::DrawCubes(&view[0][0], &proj[0][0], &cube[0][0], 1);
            //ImGuizmo::ViewManipulate(&view[0][0], 35.f, ImVec2(viewport.x, viewport.y), ImVec2(128, 128), 0x000000ff);

            // HACK: Reset hovered window
            g.HoveredWindow = hovered;

            // Draw wireframe of current selection
            render::Render& r = Engine.Render;
            Entity ent = Selection.Active();
            if (ent && ent.HasComponent<MeshRenderer>()) {
                if (ent.HasComponent<Transform>()) {
                    mat4x4 matrix = ent.GetComponent<Transform>().GetTransformMatrix();
                    r.SetTransform(matrix);
                }
                r.SetDepthTest(render::CompareFunc::LessEqual);
                r.SetShader(Editor.sh_Wireframe);
                r.SetPolygonMode(render::PolygonMode::Lines);
                r.DrawMesh(ent.GetComponent<MeshRenderer>().mesh);
                r.SetPolygonMode(render::PolygonMode::Fill);
                r.SetDepthTest(render::CompareFunc::Less);
            }
        }

        void Draw() override
        {
            ResetPadding();

            if (ImGui::BeginMenuBar())
            {
                CoordinateSpacePicker();

                ImGui::SameLine(ImGui::GetWindowWidth() - 40);
                if (ImGui::BeginMenu(ICON_MC_VIDEO " " ICON_MC_MENU_DOWN))
                {
                    Camera& camera = Editor.editorCamera.GetComponent<Camera>();
                    ImGui::TextUnformatted("Scene Camera");
                    ImGui::InputFloat("FOV", &camera.fieldOfView);
                    ImGui::InputFloat("Speed", &cameraSpeed);
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }
            NoPadding();

            // Return if window is collapsed
            if (!CheckResize()) {
                return;
            }

            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 size = ImGui::GetContentRegionAvail();
            ImVec2 max = ImVec2(pos.x + size.x, pos.y + size.y);
            
            if (ImGui::IsMouseHoveringRect(pos, max) && !ImGui::GetIO().KeyCtrl)
            {
                Transform& transform = Editor.editorCamera.GetComponent<Transform>();

                // TODO: Make Z toggle instead of hold
                if (Mouse.GetButtonDown(Mouse::Right) || Keyboard.GetKeyDown(Key::Z))
                {
                    Cursor.SetMode(Cursor::Locked);
                    Cursor.SetVisible(false);
                }

                if (Mouse.GetButton(Mouse::Right) || Keyboard.GetKey(Key::Z))
                {
                    // Mouselook
                    vec3 euler = transform.GetEulerAngles();
                    vec2 mouse = vec2(Mouse.GetMotion()) / vec2(2);
                    transform.SetEulerAngles(vec3(euler.x + mouse.y, euler.y + mouse.x, euler.z));
                }

                if (Mouse.GetButtonUp(Mouse::Right) || Keyboard.GetKeyUp(Key::Z))
                {
                    Cursor.SetMode(Cursor::Normal);
                    Cursor.SetVisible(true);
                }

                // WASD. TODO: Virtual axes, arrow keys
                float w = Keyboard.GetKey(Key::W) ? 1.f : 0.f;
                float s = Keyboard.GetKey(Key::S) ? 1.f : 0.f;
                float a = Keyboard.GetKey(Key::A) ? 1.f : 0.f;
                float d = Keyboard.GetKey(Key::D) ? 1.f : 0.f;

                transform.position += transform.Forward() * (w-s) * cameraSpeed;
                transform.position += transform.Right() * (d-a) * cameraSpeed;
            }

            // Copy from scene view render target into viewport
            ImGui::GetWindowDrawList()->AddImage(
                Editor.rt_SceneView->GetTexture(),
                pos, max,
                ImVec2(0, 0), ImVec2(1, 1)
            );

            viewport = Rect(pos.x, pos.y, size.x, size.y);

            Toolbar();
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
            if (ImGui::BeginMenu(label.c_str())) {
                for (int j = 0; j < std::size(items); j++) {
                    if (ImGui::MenuItem(items[j])) {
                        space = Space(j);
                    }
                }
                ImGui::EndMenu();
            }
        }

        void Toolbar()
        {
            ImGui::Spacing();

            ImGui::Indent(5.f);
            ToolbarButton(ICON_MC_ARROW_ALL, Tool::Translate);
            ToolbarButton(ICON_MC_AUTORENEW, Tool::Rotate);
            ToolbarButton(ICON_MC_RESIZE, Tool::Scale);
            ToolbarButton(ICON_MC_ALPHA_U_BOX_OUTLINE, Tool::Universal);
            ImGui::Unindent(5.f);

        }

        void ToolbarButton(const char* label, Tool tool)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.f, 0.f));
            ImGuiCol col = activeTool == tool ? ImGuiCol_TabActive : ImGuiCol_WindowBg;
            ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(col));
            
            if (ImGui::Button(label)) {
                activeTool = tool;
            }

            ImGui::PopStyleColor();
            ImGui::PopStyleVar();
        }

    };
}
