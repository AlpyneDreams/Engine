#include "common/Common.h"
#include "Common.h"

#include <imgui.h>
#include "impl/fonts/roboto_regular.ttf.h"

namespace engine
{
    void ImGUI::Setup()
    {
        ImGuiIO& io = ImGui::GetIO();

        ImFontConfig config;
        config.FontDataOwnedByAtlas = false;

        io.Fonts->AddFontFromMemoryTTF((void*)s_robotoRegularTTF, sizeof(s_robotoRegularTTF), 18, &config);
    }
}