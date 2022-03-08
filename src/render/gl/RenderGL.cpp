#include "common/Common.h"
#include "common/Filesystem.h"
#include "platform/Platform.h"

#include "platform/Window.h"
#include "render/Render.h"

#include <glad/glad.h>
#include <GL/gl.h>

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
        GLbitfield clearBuffers = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

        // Temp
        uint vbo, vao, ebo;
        Shader* shader;
    protected:
        void Init(Window* window)
        {
            auto [width, height] = window->GetSize();
            glViewport(0, 0, width, height);

            // Handle window resize
            window->SetResizeCallback([=, this](uint width, uint height) {
                glViewport(0, 0, width, height);
            });


            shader = LoadShader("triangle.vert", "triangle.frag");

            /*float vertices[] = {
                // positions         // colors
                0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
                -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
                0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
            };*/

            float vertices[] = {
                0.5f,  0.5f, 0.0f,  // top right
                0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f,  0.5f, 0.0f   // top left 
            };

            uint indices[] = {  // note that we start from 0!
            //    0, 1, 2
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
            };

            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
            //glEnableVertexAttribArray(1);

            // Permitted by the glVertexAttribPointer call
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Unbind VAO
            glBindVertexArray(0);
            // Unbind EBO (must do this after unbinding VAO)
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            // Wireframe
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        }

        void BeginFrame()
        {
            // Temp
            SetClearColor(true, Color(0.2f, 0.3f, 0.3f, 1.0f));
            //

            glClear(clearBuffers);

            // Temp
            SetShader(shader);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            //


        }

        void EndFrame()
        {
        }

        void Shutdown()
        {
            glDeleteVertexArrays(1, &vao);
            glDeleteBuffers(1, &vbo);
            glDeleteBuffers(1, &ebo);
            delete shader;
        }

    public:
        void UploadMesh(Mesh* mesh)
        {
        }

        Handle* CreateIndexBuffer(uint* indices, size_t size)
        {
            uint ebo;
            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            return nullptr;
        }

        Shader* LoadShader(const char* vertexShader, const char* pixelShader)
        {
            auto loadShader = [](const char* name, GLenum type) -> uint {
                auto path = std::string("core/shaders/glsl/") + name;
                auto [buffer, len] = fs::readFile(path.c_str());
                uint id = glCreateShader(type);
                glShaderSource(id, 1, &buffer, NULL);
                glCompileShader(id);

                int success;
                glGetShaderiv(id, GL_COMPILE_STATUS, &success);
                if (!success) {
                    char infoLog[512];
                    glGetShaderInfoLog(id, 512, NULL, infoLog);
                    fprintf(stderr, "[GL] Failed to compile shader '%s'! %s\n", name, infoLog);
                }

                return id;
            };

            uint vert = loadShader(vertexShader, GL_VERTEX_SHADER);
            uint frag = loadShader(pixelShader, GL_FRAGMENT_SHADER);

            uint program = glCreateProgram();
            glAttachShader(program, vert);
            glAttachShader(program, frag);
            glLinkProgram(program);

            int success;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetProgramInfoLog(program, 512, NULL, infoLog);
                fprintf(stderr, "[GL] Failed to link shader program '%s/%s'! %s\n", vertexShader, pixelShader, infoLog);
            }

            glDeleteShader(vert);
            glDeleteShader(frag);

            return new ShaderGL {.program = program};
        }


        void SetShader(Shader* shader)
        {
            glUseProgram(static_cast<ShaderGL*>(shader)->program);
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
            if (clear)  clearBuffers |= GL_COLOR_BUFFER_BIT;
            else        clearBuffers &= ~GL_COLOR_BUFFER_BIT;
            glClearColor(color.r, color.g, color.b, color.a);
        }

        void SetClearDepth(bool clear, float depth)
        {
            if (clear)  clearBuffers |= GL_DEPTH_BUFFER_BIT;
            else        clearBuffers &= ~GL_DEPTH_BUFFER_BIT;
            glClearDepth(depth);
        }
    };
    
    Render* Render::Create()
    {
        return new RenderGL();
    }

}