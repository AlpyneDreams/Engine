#pragma once

#include "common/Common.h"
#include "engine/System.h"
#include "math/Math.h"
#include "Common.h"
#include <imgui.h>
#include <string>

namespace engine::GUI
{
    struct Window : public System
    {
        std::string name = "##"; // (unnamed)
        uint width = 512;
        uint height = 512;
        bool open = true;
        ImGuiWindowFlags flags;

        Window() {}
        Window(const char* name, uint width, uint height, bool open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
          : name(name), width(width), height(height), open(open), flags(flags) {}

        Window(const char* icon, const char* name, uint width, uint height, bool open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
          : Window(icon, name, name, width, height, open, flags) {}
        
        Window(const char* icon, const char* title, const char* id, uint width, uint height, bool open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
          : Window((std::string(icon) + " " + title + "###" + id).c_str(), width, height, open, flags) {}

        void Update() override
        {
            if (!open)
                return;
            
            ImGui::SetNextWindowSize(ImVec2(float(width), float(height)), ImGuiCond_FirstUseEver);
            PreDraw();
            if (ImGui::Begin(name.c_str(), &open, flags))
            {
                Draw();
            }

            PostDraw();
            ImGui::End();
        }
        
        // Subclasses will override this
        virtual void PreDraw() {}
        virtual void Draw() {}
        virtual void PostDraw() {}
    };
}