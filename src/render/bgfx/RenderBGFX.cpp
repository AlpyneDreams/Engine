#include "common/Common.h"
#include "platform/Platform.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <stdexcept>

#include "platform/Window.h"
#include "render/Render.h"

namespace engine
{
    struct RenderState
    {
        bgfx::ViewId clearView;
    };

    Render::Render() { state = new RenderState(); }

    void Render::Init(Window* window)
    {
        // Signal to BGFX not to create a render thread
        bgfx::renderFrame();
        
        bgfx::Init init;

        init.platformData.ndt = window->GetNativeDisplay();
        init.platformData.nwh = window->GetNativeWindow();

        auto [width, height] = window->GetSize();
        init.resolution.width = uint32(width);
        init.resolution.height = uint32(height);

        init.resolution.reset = BGFX_RESET_VSYNC;

        // Handle window resize
        window->SetResizeCallback([=](uint width, uint height) {
            bgfx::reset(uint32(width), uint32(height), BGFX_RESET_VSYNC);
            bgfx::setViewRect(state->clearView, 0, 0, bgfx::BackbufferRatio::Equal);
        });

        if (!bgfx::init(init)) {
            throw std::runtime_error("[BGFX] Failed to initialize!");
        }

        state->clearView = 0;
        bgfx::setViewClear(state->clearView, BGFX_CLEAR_COLOR);
        bgfx::setViewRect(state->clearView, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    void Render::Update()
    {
        bgfx::touch(state->clearView);

        bgfx::dbgTextClear();
		//bgfx::dbgTextImage(bx::max<uint16_t>(uint16_t(width / 2 / 8), 20) - 20, bx::max<uint16_t>(uint16_t(height / 2 / 16), 6) - 6, 40, 12, s_logo, 160);
		//bgfx::dbgTextPrintf(0, 0, 0x0f, "Press F1 to toggle stats.");
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
		bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
		const bgfx::Stats* stats = bgfx::getStats();
		bgfx::dbgTextPrintf(0, 2, 0x0f, "Backbuffer %dW x %dH in pixels, debug text %dW x %dH in characters.", stats->width, stats->height, stats->textWidth, stats->textHeight);
        bgfx::setDebug(BGFX_DEBUG_TEXT);

        bgfx::frame();
    }

    void Render::Shutdown()
    {
        bgfx::shutdown();
    }
}