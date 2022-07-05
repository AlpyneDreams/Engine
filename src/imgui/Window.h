#pragma once

#include "common/Common.h"
#include "engine/System.h"
#include "math/Math.h"
#include "Common.h"
#include <imgui.h>

namespace engine::GUI
{
    struct Window : public System
    {
        const char* name = "##"; // (unnamed)
        uint width = 512;
        uint height = 512;
        bool open = true;
        ImGuiWindowFlags flags;

        Window() {}
        Window(const char* name, uint width, uint height, bool open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
          : name(name), width(width), height(height), open(open), flags(flags) {}

        void Update() override
        {
            if (!open)
                return;
            
            ImGui::SetNextWindowSize(ImVec2(float(width), float(height)), ImGuiCond_FirstUseEver);
            PreDraw();
            if (ImGui::Begin(name, &open, flags))
            {
                Draw();
            }

            ImGui::End();
            PostDraw();
        }
        
        // Subclasses will override this
        virtual void PreDraw() {}
        virtual void Draw() {}
        virtual void PostDraw() {}
    };
}