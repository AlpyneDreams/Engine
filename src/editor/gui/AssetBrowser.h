#pragma once

#include "imgui/Window.h"
#include "imgui/IconsMaterialCommunity.h"

#include "input/Keyboard.h"

namespace engine::editor
{
    struct AssetBrowser : public GUI::Window
    {
        AssetBrowser() : GUI::Window(ICON_MC_FOLDER " Files", 512, 512, false) {}

        void Update() final override
        {
            if (ImGui::GetIO().KeyCtrl && Keyboard.GetKeyUp(Key::Space)) {
                open = !open;
            }
            Window::Update();
        }

        void Draw() override
        {
        }
    };
}
