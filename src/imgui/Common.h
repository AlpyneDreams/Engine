#pragma once

#include <string_view>

namespace engine
{
    namespace GUI
    {
        void Setup();
        void ShowDemoWindow();

        // Aligned (left or right) item label (use before input controls)
        void ItemLabel(std::string_view title, bool right = false);
    }
}