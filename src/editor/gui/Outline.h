#pragma once

#include "entity/Common.h"
#include "imgui.h"
#include "imgui/Window.h"
#include "editor/Selection.h"
#include "entity/Scene.h"
#include "entity/Entity.h"

namespace engine::editor
{
    struct Outline : public GUI::Window
    {
        Outline() : GUI::Window("Outline", 512, 512, true, ImGuiWindowFlags_MenuBar) {}

        void AddEntity()
        {
            Entity ent = Scene::World.CreateEntity();
            Selection.Select(ent);
        }

        void Draw() override
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("Add"))
                {
                    if (ImGui::MenuItem("Entity")) {
                        AddEntity();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            Scene::World.ents.each([this](auto& ent)
            {
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;

                if (Selection.Selected(ent)) {
                    flags |= ImGuiTreeNodeFlags_Selected;
                }

                if (ImGui::TreeNodeEx(&ent, flags, "Entity %d", ent))
                {
                    if (ImGui::IsItemClicked())
                    {
                        if (ImGui::GetIO().KeyCtrl) {
                            Selection.Toggle(ent);
                        } else {
                            Selection.Select(ent);
                        }
                    }
                    ImGui::TreePop();
                }
                DrawContextMenu(ent);
            });

            DrawContextMenu();
        }

        void DrawContextMenu(EntityID id = EntityNull)
        {
            auto flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
            bool window = id == EntityNull;
            if (window ? ImGui::BeginPopupContextWindow(NULL, flags) : ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete", NULL, false, !window)) {        
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Create Entity")) {
                    AddEntity();
                }
                ImGui::EndPopup();
            }
        }
    };
}
