#pragma once

#include <string_view>

struct ImFont;

namespace engine
{
    namespace GUI
    {
        void Setup();
        void ShowDemoWindow();

        inline ImFont* FontMonospace = nullptr;

        // Aligned (left or right) item label (use before input controls)
        void ItemLabel(std::string_view title, bool right = false);
    }
}