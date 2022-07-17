#pragma once

#include "platform/Window.h"
#include "math/Math.h"
#include "math/Color.h"

namespace engine { struct RenderSystem; struct Mesh; }

namespace engine::render
{
    struct Shader {};

    struct RenderTarget
    {
        // TODO: MRT attachment configuration
        virtual void* GetTexture() const = 0;
        virtual uint2 GetSize() const = 0;
        virtual void Resize(uint width, uint height) = 0;
    };

    struct Handle {};

    enum class CompareFunc { Disabled, Never, Always, Less, LessEqual, Greater, GreaterEqual, Equal, NotEqual };
    enum class PolygonMode { Points, Lines, Fill };

    class Render
    {
        friend struct ::engine::RenderSystem;
    protected:
        virtual void Init(Window* window) = 0;
        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;
        virtual void Shutdown() = 0;
    public:
        static Render* Create();
        virtual ~Render() {}

    // Resource Uploading //

        virtual void UploadMesh(Mesh* mesh) = 0;

    // Resource Creation and Loading //

        // Default format with depth buffer.
        virtual RenderTarget* CreateRenderTarget(uint width, uint height) = 0;

        virtual Shader* LoadShader(const char* vertexShader, const char* pixelShader) = 0; // TODO: Probably replace this

    // Per-Camera State //

        // nullptr = default backbuffer
        virtual void SetRenderTarget(RenderTarget* rt) = 0;
        virtual void SetViewTransform(mat4x4& view, mat4x4& proj) = 0;
        virtual void SetClearColor(bool clear, Color color = Colors.Black) = 0;
        virtual void SetClearDepth(bool clear, float depth = 1.0f) = 0;

    // Per-Object State //

        virtual void SetDepthTest(CompareFunc func) = 0;
        virtual void SetPolygonMode(PolygonMode mode) = 0;
        virtual void SetTransform(mat4x4& matrix) = 0;
        virtual void SetShader(Shader* shader) = 0;

    // Draw Calls //

        virtual void DrawMesh(Mesh* mesh) = 0;

    };
}