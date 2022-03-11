#include "common/Common.h"
#include "common/Filesystem.h"
#include "platform/Platform.h"

#include <bgfx/bgfx.h>
#include <bx/string.h>
#include <bgfx/platform.h>

#include <stdexcept>
#include <unordered_map>
#include <map>
#include <typeindex>

#include "platform/Window.h"
#include "render/Render.h"
#include "core/Mesh.h"

namespace engine::render
{
    // Maps an integral type to bgfx::AttribType
    static std::unordered_map<std::type_index, bgfx::AttribType::Enum> bgfxAttribTypes {
        {std::type_index(typeid(uint8)), bgfx::AttribType::Uint8},
        //{std::type_index(typeid(uint10)), bgfx::AttribType::Uint10},
        {std::type_index(typeid(int16)), bgfx::AttribType::Int16},
        //{std::type_index(typeid(half)), bgfx::AttribType::Half},
        {std::type_index(typeid(float)), bgfx::AttribType::Float},
    };

    struct ShaderBGFX final : public Shader
    {
        bgfx::ProgramHandle program;

        ~ShaderBGFX() {
            bgfx::destroy(program);
        }
    };

    struct HandleBGFX final : public Handle
    {
        union {
            bgfx::VertexBufferHandle vb;
            bgfx::IndexBufferHandle ib;
        };
    };

    class RenderBGFX final : public Render
    {
        struct RenderState {
            uint width, height;
            bgfx::ViewId clearView;

            struct ClearState {
                uint16 flags = BGFX_CLEAR_NONE;
                uint32 rgba  = 0x000000FF;
                float depth = 1.0f;
                uint8 stencil = 0;
            } clear;

            bgfx::ProgramHandle currentProgram;
        } state;

    protected:
        void Init(Window* window)
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

            state.width = width, state.height = height;

            // Handle window resize
            window->SetResizeCallback([=, this](uint width, uint height) {
                bgfx::reset(uint32(width), uint32(height), BGFX_RESET_VSYNC);
                bgfx::setViewRect(state.clearView, 0, 0, bgfx::BackbufferRatio::Equal);
            });

            if (!bgfx::init(init)) {
                throw std::runtime_error("[BGFX] Failed to initialize!");
            }

            state.clearView = 0;
            bgfx::setViewRect(state.clearView, 0, 0, bgfx::BackbufferRatio::Equal);
        }

        void BeginFrame()
        {
            // Always clear this view even if no draw calls are made
            bgfx::touch(state.clearView);
        }

        void EndFrame()
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

        void Shutdown()
        {
            bgfx::shutdown();
        }

    private:
        bgfx::Attrib::Enum GetAttribute(VertexAttribute::Mode mode) {
            switch (mode) {
                case VertexAttribute::Position: return bgfx::Attrib::Position;
                case VertexAttribute::Normal:   return bgfx::Attrib::Normal;
                case VertexAttribute::Tangent:  return bgfx::Attrib::Tangent;
                case VertexAttribute::Bitangent: return bgfx::Attrib::Bitangent;
                case VertexAttribute::Color:    return bgfx::Attrib::Color0;
                case VertexAttribute::Indices:  return bgfx::Attrib::Indices;
                case VertexAttribute::Weight:   return bgfx::Attrib::Weight;

                default:
                case VertexAttribute::TexCoord: return bgfx::Attrib::TexCoord0;
            }
        }

    public:
        void UploadMesh(Mesh* mesh)
        {
            bgfx::VertexLayout layout;
            layout.begin();

            for (auto attr : mesh->layout.Attributes()) {
                layout.add(GetAttribute(attr.mode), attr.dimension, bgfxAttribTypes[std::type_index(attr.type)], attr.normalized);
            }
            layout.end();

            for (auto& group : mesh->groups) {

                auto vb = bgfx::createVertexBuffer(
                    bgfx::makeRef(group.vertices.vertices, group.vertices.Size()),
                    layout
                );

                HandleBGFX* vh = new HandleBGFX();
                vh->vb = vb;
                group.vertices.handle = vh;
                
                auto ib = bgfx::createIndexBuffer(
                    bgfx::makeRef(group.indices.indices, group.indices.Size()),
                    group.indices.type == IndexBuffer::UInt32 ? BGFX_BUFFER_INDEX32 : 0
                );

                HandleBGFX* ih = new HandleBGFX();
                ih->ib = ib;
                group.indices.handle = ih;
            }
        }

        Shader* LoadShader(const char* vertexShader, const char* pixelShader)
        {
            bgfx::ShaderHandle vert = LoadShaderModule(vertexShader);
            bgfx::ShaderHandle frag = BGFX_INVALID_HANDLE;
            if (pixelShader != NULL) {
                frag = LoadShaderModule(pixelShader);
            }

            ShaderBGFX* shader = new ShaderBGFX();
            shader->program = bgfx::createProgram(vert, frag, true /* destroy shaders when program is destroyed */);
            return shader;
        }


        void SetShader(Shader* shader)
        {
            state.currentProgram = static_cast<ShaderBGFX*>(shader)->program;
        }

        void DrawMesh(Mesh* mesh)
        {
            for (auto& group : mesh->groups) {
                auto vb = static_cast<HandleBGFX*>(group.vertices.handle)->vb;
                auto ib = static_cast<HandleBGFX*>(group.indices.handle)->ib;
                bgfx::setVertexBuffer(0, vb);
                bgfx::setIndexBuffer(ib);

                bgfx::submit(0, state.currentProgram);
            }
        }


        void Submit()
        {
            bgfx::submit(0, state.currentProgram);
        }


        float GetAspectRatio()
        {
            return float(state.width) / float(state.height);
        }

        void SetViewTransform(hlslpp::float4x4& view, hlslpp::float4x4& proj)
        {
            bgfx::setViewTransform(0, &view._m00, &proj._m00);
        }

        void SetTransform(hlslpp::float4x4& matrix)
        {
            bgfx::setTransform(&matrix._m00);
        }

        static inline void UpdateClearState(const RenderState& state)
        {
            bgfx::setViewClear(state.clearView, state.clear.flags, state.clear.rgba, state.clear.depth, state.clear.stencil);
        }

        void SetClearColor(bool clear, Color color)
        {
            if (clear)  state.clear.flags |= BGFX_CLEAR_COLOR;
            else        state.clear.flags &= ~BGFX_CLEAR_COLOR;
            state.clear.rgba = color.Pack();
            UpdateClearState(state);
        }

        void SetClearDepth(bool clear, float depth)
        {
            if (clear)  state.clear.flags |= BGFX_CLEAR_DEPTH;
            else        state.clear.flags &= ~BGFX_CLEAR_DEPTH;
            state.clear.depth = depth;
            UpdateClearState(state);
        }

    private:
        const bgfx::Memory* LoadMem(const char* filePath)
        {
            auto [buffer, len] = fs::readFile(filePath);
            return bgfx::copy(buffer, len);
        }

        bgfx::ShaderHandle LoadShaderModule(const char* _name)
        {
            char filePath[512];

            const char* shaderPath = "???";

            switch (bgfx::getRendererType() )
            {
            case bgfx::RendererType::Noop:
            case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
            case bgfx::RendererType::Direct3D11:
            case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
            case bgfx::RendererType::Agc:
            case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
            case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
            case bgfx::RendererType::Nvn:        shaderPath = "shaders/nvn/";   break;
            case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
            case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
            case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
            case bgfx::RendererType::WebGPU:     shaderPath = "shaders/spirv/"; break;

            case bgfx::RendererType::Count:
                BX_ASSERT(false, "You should not be here!");
                break;
            }

            bx::strCopy(filePath, BX_COUNTOF(filePath), "core/");
            bx::strCat(filePath, BX_COUNTOF(filePath), shaderPath);
            bx::strCat(filePath, BX_COUNTOF(filePath), _name);
            bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

            bgfx::ShaderHandle handle = bgfx::createShader(LoadMem(filePath));
            bgfx::setName(handle, _name);

            return handle;
        }

    };
    
    Render* Render::Create()
    {
        return new RenderBGFX();
    }

}