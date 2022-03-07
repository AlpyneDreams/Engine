#include "common/Common.h"
#include "platform/Platform.h"

#include <stdexcept>

#include "platform/Window.h"
#include "render/Render.h"

#include <raylib.h>

namespace engine::render
{
    class RenderRaylib final : public Render
    {
        struct RenderState {
            uint width, height;

            bool    clear = true;
            ::Color clearColor = RAYWHITE;
            
        } state;
        

    protected:
        void Init(Window* window)
        {
            SetTargetFPS(60); // Don't melt GPU
        }

        void BeginFrame()
        {
            BeginDrawing();
            {
                if (state.clear)
                    ClearBackground(state.clearColor);
            }
        }

        void EndFrame()
        {
            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            EndDrawing();
        }

        void Shutdown() {}

    public:

        float GetAspectRatio()
        {
            return float(state.width) / float(state.height);
        }

        void SetViewTransform(hlslpp::float4x4& view, hlslpp::float4x4& proj)
        {
            //bgfx::setViewTransform(0, &view._m00, &proj._m00);
        }

        void SetTransform(hlslpp::float4x4& matrix)
        {
            //bgfx::setTransform(&matrix._m00);
        }

        static inline void UpdateClearState(const RenderState& state)
        {
            //bgfx::setViewClear(state.clearView, state.clear.flags, state.clear.rgba, state.clear.depth, state.clear.stencil);
        }

        void SetClearColor(bool clear, Color color)
        {
            state.clear = clear;
            state.clearColor = color;
        }

        void SetClearDepth(bool clear, float depth)
        {
            /*if (clear)  state.clear.flags |= BGFX_CLEAR_DEPTH;
            else        state.clear.flags &= ~BGFX_CLEAR_COLOR;
            state.clear.depth = depth;
            UpdateClearState(state);*/
        }
    };
    
    Render* Render::Create()
    {
        return new RenderRaylib();
    }

}