#include "common/Common.h"
#include "Common.h"

#include <imgui.h>
#include <imgui_internal.h>
#include "impl/fonts/roboto_regular.ttf.h"

namespace engine
{
    void GUI::Setup()
    {
        //ImGuiIO& io = ImGui::GetIO();

        //ImFontConfig config;
        //config.FontDataOwnedByAtlas = false;

        //io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTTF, sizeof(s_robotoRegularTTF), 18, &config);
    }

    void GUI::ShowDemoWindow()
    {
        ImGui::ShowDemoWindow();
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