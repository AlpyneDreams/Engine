#include "common/Common.h"
#include "common/Filesystem.h"
#include "platform/Platform.h"

#include "platform/Window.h"
#include "render/Render.h"

//#include <glad/glad.h>
//#include <GL/gl.h>

#include <GL/OOGL.hpp>

namespace engine::render
{
    struct ShaderGL final : public Shader
    {
        uint program;

        ~ShaderGL() {
            glDeleteProgram(program);
        }
    };

    class RenderGL final : public Render
    {
        GL::Context* gl;
        GL::Program* program;
        GL::VertexArray* vao;
        GL::VertexBuffer* vbo;
    protected:
        void Init(Window* win)
        {
            GL::Window* window = static_cast<GL::Window*>(win->GetHandle());
            gl = &window->GetContext(24);

            GL::Shader vert( GL::ShaderType::Vertex, "#version 150\nin vec2 position; void main() { gl_Position = vec4( position, 0.0, 1.0 ); }" );
            GL::Shader frag( GL::ShaderType::Fragment, "#version 150\nout vec4 outColor; void main() { outColor = vec4( 1.0, 0.0, 0.0, 1.0 ); }" );
            program = new GL::Program( vert, frag );

            const float vertices[] = {
                -0.5f,  0.5f,
                0.5f,  0.5f,
                0.5f, -0.5f
            };
            vbo = new GL::VertexBuffer( vertices, sizeof( vertices ), GL::BufferUsage::StaticDraw );

            vao = new GL::VertexArray();
            vao->BindAttribute( program->GetAttribute( "position" ), *vbo, GL::Type::Float, 2, 0, 0 );
        }

        void BeginFrame()
        {
            gl->Clear();

            gl->DrawArrays( *vao, GL::Primitive::Triangles, 0, 3 );
        }

        void EndFrame()
        {
        }

        void Shutdown()
        {
        }

    public:
        void UploadMesh(Mesh* mesh)
        {
        }

        Handle* CreateIndexBuffer(uint* indices, size_t size)
        {
            return nullptr;
        }

        Shader* LoadShader(const char* vertexShader, const char* pixelShader)
        {
            return nullptr;
        }


        void SetShader(Shader* shader)
        {
            
        }

        void DrawMesh(Mesh* mesh)
        {
        }

        void Submit()
        {
        }


        float GetAspectRatio()
        {
        }

        void SetViewTransform(hlslpp::float4x4& view, hlslpp::float4x4& proj)
        {
        }

        void SetTransform(hlslpp::float4x4& matrix)
        {
        }

        void SetClearColor(bool clear, Color color)
        {
        }

        void SetClearDepth(bool clear, float depth)
        {
        }
    };
    
    Render* Render::Create()
    {
        return new RenderGL();
    }

}