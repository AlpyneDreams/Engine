#pragma once

#include "imgui/Window.h"
#include "editor/Selection.h"
#include "entity/Scene.h"
#include "entity/Entity.h"

namespace engine::editor
{
    struct Outline : public GUI::Window
    {
        Outline() : GUI::Window("Outline", 512, 512, true) {}

        void Draw() override
        {
            Scene::World.ents.each([this](auto& ent)
            {
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;

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
            });

            if (ImGui::Button("Add Entity")) {
                Entity ent = Scene::World.CreateEntity();
                Selection.Select(ent);
            }
            
        }
    };
}
