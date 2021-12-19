#include "common/Common.h"
#include "platform/Platform.h"

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <stdexcept>

#include "platform/Window.h"
#include "render/Render.h"

namespace engine::render
{
    struct RenderState
    {
        uint width, height;
        bgfx::ViewId clearView;

        struct ClearState {
            uint16 flags = BGFX_CLEAR_NONE;
            uint32 rgba  = 0x000000FF;
            float depth = 1.0f;
            uint8 stencil = 0;
        } clear;
    };

    Render::Render() { state = new RenderState(); }
    Render::~Render() { delete state; }

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

        state->width = width, state->height = height;

        // Handle window resize
        window->SetResizeCallback([=](uint width, uint height) {
            bgfx::reset(uint32(width), uint32(height), BGFX_RESET_VSYNC);
            bgfx::setViewRect(state->clearView, 0, 0, bgfx::BackbufferRatio::Equal);
        });

        if (!bgfx::init(init)) {
            throw std::runtime_error("[BGFX] Failed to initialize!");
        }

        state->clearView = 0;
        bgfx::setViewRect(state->clearView, 0, 0, bgfx::BackbufferRatio::Equal);
    }

    void Render::BeginFrame()
    {
        // Always clear this view even if no draw calls are made
        bgfx::touch(state->clearView);
    }

    void Render::EndFrame()
    {
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

    float Render::GetAspectRatio()
    {
        return float(state->width) / float(state->height);
    }

    void Render::SetViewTransform(hlslpp::float4x4& view, hlslpp::float4x4& proj)
    {
        bgfx::setViewTransform(0, &view._m00, &proj._m00);
    }

    void Render::SetTransform(hlslpp::float4x4& matrix)
    {
        bgfx::setTransform(&matrix._m00);
    }

    static inline void UpdateClearState(const RenderState& state)
    {
        bgfx::setViewClear(state.clearView, state.clear.flags, state.clear.rgba, state.clear.depth, state.clear.stencil);
    }

    void Render::SetClearColor(bool clear, Color color)
    {
        if (clear)  state->clear.flags |= BGFX_CLEAR_COLOR;
        else        state->clear.flags &= ~BGFX_CLEAR_COLOR;
        state->clear.rgba = color.Pack();
        UpdateClearState(*state);
    }

    void Render::SetClearDepth(bool clear, float depth)
    {
        if (clear)  state->clear.flags |= BGFX_CLEAR_DEPTH;
        else        state->clear.flags &= ~BGFX_CLEAR_COLOR;
        state->clear.depth = depth;
        UpdateClearState(*state);
    }
}