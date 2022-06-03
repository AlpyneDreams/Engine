#pragma once

#include "platform/Window.h"
#include "math/Math.h"
#include "math/Color.h"

namespace engine { struct RenderSystem; struct Mesh; }

namespace engine::render
{
    class Shader {};
    class Handle {};

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

        virtual void UploadMesh(Mesh* mesh) = 0;

        virtual Shader* LoadShader(const char* vertexShader, const char* pixelShader) = 0; // TODO: Probably replace this
        virtual void    SetShader(Shader* shader) = 0;

        virtual void DrawMesh(Mesh* mesh) = 0;

        virtual void Submit() = 0;

        virtual float GetAspectRatio() = 0;

        virtual void SetViewTransform(Matrix4x4& view, Matrix4x4& proj) = 0;
        virtual void SetTransform(Matrix4x4& matrix) = 0;

        virtual void SetClearColor(bool clear, Color color = Colors.Black) = 0;
        virtual void SetClearDepth(bool clear, float depth = 1.0f) = 0;
    };
}