#pragma once

#include <string_view>

#include "render/Texture.h"

struct ImFont;

namespace engine
{
    namespace GUI
    {
        void Setup();
        void Update();
        void Render();

        inline ImFont* FontMonospace = nullptr;

        // Aligned (left or right) item label (use before input controls)
        void ItemLabel(std::string_view title, bool right = false);

        bool MenuBarButton(std::string_view title);

        bool Thumbnail(const char* name, Texture* icon = nullptr, bool selected = false);
    }
}