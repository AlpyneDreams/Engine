#pragma once

#include "imgui/Window.h"
#include "editor/Selection.h"
#include "entity/Entity.h"

namespace engine::editor
{
    struct Inspector : public GUI::Window
    {
        Inspector() : GUI::Window("Inspector", 512, 512, true) {}

        void Draw() override
        {
            EntityID ent = Selection.Active();
            if (ent == EntityNull) return;

            ImGui::Text("Entity %d", ent);
            
            if (ImGui::Button("Add Component")) {
            }
            
        }
    };
}
