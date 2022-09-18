#pragma once

#include "common/Common.h"
#include "engine/System.h"
#include "math/Math.h"
#include "Common.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <string>

namespace engine::GUI
{
    struct Window : public System
    {
        std::string name = "##"; // (unnamed)
        uint width = 512;
        uint height = 512;
        bool open = true;
        bool visible = false;
        ImGuiWindowFlags flags;

    private:
        ImGuiWindow* m_window = nullptr;
    public:

        Window() {}
        Window(const char* name, uint width, uint height, bool open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
          : name(name), width(width), height(height), open(open), flags(flags) {}

        Window(const char* icon, const char* name, uint width, uint height, bool open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
          : Window(icon, name, name, width, height, open, flags) {}
        
        Window(const char* icon, const char* title, const char* id, uint width, uint height, bool open = true, ImGuiWindowFlags flags = ImGuiWindowFlags_None)
          : Window((std::string(icon) + " " + title + "###" + id).c_str(), width, height, open, flags) {}

        void Update() override
        {
            if (!open) {
                visible = false;
                return;
            }
            
            ImGui::SetNextWindowSize(ImVec2(float(width), float(height)), ImGuiCond_FirstUseEver);
            PreDraw();
            if (ImGui::Begin(name.c_str(), &open, flags))
            {
                m_window = ImGui::GetCurrentWindow();
                visible = true;
                Draw();
            }
            else
            {
                visible = false;
            }

            PostDraw();
            ImGui::End();
        }

        // Focuses the window if it's hidden, 
        // otherwise toggles it open/closed.
        void ToggleOrFocus()
        {
            if (visible) {
                open = false;
            } else {
                open = true;
                if (m_window)
                    ImGui::FocusWindow(m_window);
            }
        }

        // Subclasses should override this.
        virtual void Draw() {}
        
        // Subclasses can override these.
        // Unlike Draw, PreDraw and PostDraw run regardless of whether
        // the window is visible, so you should check this->visible in them.
        virtual void PreDraw() {}
        virtual void PostDraw() {}
    };
}