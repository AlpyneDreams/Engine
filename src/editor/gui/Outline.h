#pragma once

#include "entity/Common.h"
#include "entity/components/Transform.h"
#include "imgui.h"
#include "imgui/Window.h"
#include "editor/Selection.h"
#include "entity/Scene.h"
#include "entity/Entity.h"

#include "imgui/IconsMaterialCommunity.h"

namespace engine::editor
{
    struct Outline : public GUI::Window
    {
        Outline() : GUI::Window("Outline", 512, 512, true, ImGuiWindowFlags_MenuBar) {}

        void AddEntity()
        {
            Entity ent = World.CreateEntity();
            Selection.Select(ent);
        }

        void Draw() override
        {
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu(ICON_MC_PLUS ICON_MC_MENU_DOWN))
                {
                    if (ImGui::MenuItem(ICON_MC_CUBE_OUTLINE " Entity")) {
                        AddEntity();
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            World.Each([this](auto& ent)
            {
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth;

                if (Selection.Selected(ent)) {
                    flags |= ImGuiTreeNodeFlags_Selected;
                }

                if (ImGui::TreeNodeEx(&ent, flags, ICON_MC_CUBE_OUTLINE " Entity %d", ent))
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

        void DrawContextMenu(Entity ent = EntityNull)
        {
            auto flags = ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems;
            bool window = !ent;
            if (window ? ImGui::BeginPopupContextWindow(NULL, flags) : ImGui::BeginPopupContextItem())
            {
                if (ImGui::MenuItem("Delete", NULL, false, !window)) {
                    ent.Delete();
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
