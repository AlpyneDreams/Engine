#include "common/Common.h"
#include "Common.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "console/Console.h"
#include "console/ConVar.h"
#include "common/Filesystem.h"

namespace engine
{
    static ImFont* AddFontFile(const char* name, float size)
    {
        std::string path = std::string("core/fonts/") + name;
        if (fs::exists(path)) {
            return ImGui::GetIO().Fonts->AddFontFromFileTTF(path.c_str(), size);
        } else {
            Console.Error("[GUI] Font file not found: " + path);
            return nullptr;
        }
    }

    void GUI::Setup()
    {
        ImGuiIO& io = ImGui::GetIO();
        io.IniFilename = "engine.ui.ini";
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        //ImFontConfig config;
        //config.FontDataOwnedByAtlas = false;
        //io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTTF, sizeof(s_robotoRegularTTF), 18, &config);

        // Default font
        AddFontFile("Roboto-Regular.ttf", 15);

        GUI::FontMonospace = AddFontFile("RobotoMono.ttf", 16);

        AddFontFile("Cousine-Regular.ttf", 15);
        AddFontFile("DroidSans.ttf", 15);
        AddFontFile("Karla-Regular.ttf", 15);
        AddFontFile("ProggyClean.ttf", 13);
        AddFontFile("Roboto-Medium.ttf", 15);

        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg]               = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TabUnfocused]           = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
    }

    static ConVar gui_demo("gui_demo", false, "Show ImGui demo window");

    void GUI::Update()
    {
        if (gui_demo) {
            ImGui::ShowDemoWindow();
        }
    }

    void GUI::Render()
    {
        static ImGuiIO& io = ImGui::GetIO();

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    ImVec2 operator-(ImVec2 lhs, ImVec2 rhs) {
        return ImVec2{lhs.x - rhs.x, lhs.y - rhs.y};
    }

    // https://github.com/ocornut/imgui/issues/3469#issuecomment-691845667
    void GUI::ItemLabel(std::string_view title, bool right)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        const ImVec2 lineStart = ImGui::GetCursorScreenPos();
        const ImGuiStyle& style = ImGui::GetStyle();
        float fullWidth = ImGui::GetContentRegionAvail().x;
        float itemWidth = ImGui::CalcItemWidth() + style.ItemSpacing.x;
        ImVec2 textSize = ImGui::CalcTextSize(&title.front(), &title.back());
        ImRect textRect;
        textRect.Min = ImGui::GetCursorScreenPos();
        if (right)
            textRect.Min.x = textRect.Min.x + itemWidth;
        textRect.Max = textRect.Min;
        textRect.Max.x += fullWidth - itemWidth;
        textRect.Max.y += textSize.y;

        ImGui::SetCursorScreenPos(textRect.Min);

        ImGui::AlignTextToFramePadding();
        // Adjust text rect manually because we render it directly into a drawlist instead of using public functions.
        textRect.Min.y += window->DC.CurrLineTextBaseOffset;
        textRect.Max.y += window->DC.CurrLineTextBaseOffset;

        ImGui::ItemSize(textRect);
        if (ImGui::ItemAdd(textRect, window->GetID(title.data(), title.data() + title.size())))
        {
            ImGui::RenderTextEllipsis(ImGui::GetWindowDrawList(), textRect.Min, textRect.Max, textRect.Max.x,
                textRect.Max.x, title.data(), title.data() + title.size(), &textSize);

            if (textRect.GetWidth() < textSize.x && ImGui::IsItemHovered())
                ImGui::SetTooltip("%.*s", (int)title.size(), title.data());
        }
        if (!right)
        {
            ImGui::SetCursorScreenPos(textRect.Max - ImVec2{0, textSize.y + window->DC.CurrLineTextBaseOffset});
            ImGui::SameLine();
        }
        else if (right)
            ImGui::SetCursorScreenPos(lineStart);
    }
}