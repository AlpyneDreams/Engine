#include "Forward.h"
#include "common/Common.h"
#include "common/Filesystem.h"
#include "math/Math.h"
#include "core/Mesh.h"


#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/string.h>

#include <cstdio>

namespace fs = engine::fs;

static const bgfx::Memory* loadMem(const char* _filePath)
{
    auto [buffer, len] = fs::readFile(_filePath);
    return bgfx::copy(buffer, len);
}


static bgfx::ShaderHandle loadShader(const char* _name)
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

	bgfx::ShaderHandle handle = bgfx::createShader(loadMem(filePath));
	bgfx::setName(handle, _name);

	return handle;
}

bgfx::ProgramHandle loadProgram(const char* _vsName, const char* _fsName)
{
	bgfx::ShaderHandle vsh = loadShader(_vsName);
	bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
	if (NULL != _fsName)
	{
		fsh = loadShader(_fsName);
	}

	return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}

namespace engine::render
{
    using Forward = ForwardRenderPipeline;

    struct ForwardState
    {
        bgfx::VertexBufferHandle vb;
        IndexBuffer* ib;
        bgfx::ProgramHandle program;
    };

    struct XYZColor {
        float x, y, z;
        uint32 rgba;
    };

    static XYZColor CubeVertices[] = {
        {-1.0f,  1.0f,  1.0f, 0xff000000 },
        { 1.0f,  1.0f,  1.0f, 0xff0000ff },
        {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
        { 1.0f, -1.0f,  1.0f, 0xff00ffff },
        {-1.0f,  1.0f, -1.0f, 0xffff0000 },
        { 1.0f,  1.0f, -1.0f, 0xffff00ff },
        {-1.0f, -1.0f, -1.0f, 0xffffff00 },
        { 1.0f, -1.0f, -1.0f, 0xffffffff },
    };

    static const uint16 CubeTriangles[] = {
        0, 1, 2, // 0
        1, 3, 2,
        4, 6, 5, // 2
        5, 6, 7,
        0, 2, 4, // 4
        4, 2, 6,
        1, 5, 3, // 6
        5, 7, 3,
        0, 4, 1, // 8
        4, 5, 1,
        2, 3, 6, // 10
        6, 3, 7,
    };

    Forward::ForwardRenderPipeline()
    {
        state = new ForwardState();
    }

    static bgfx::VertexLayout layout;

    void Forward::Init(Render& r)
    {
        r.SetClearColor(true, Color(0.2, 0.2, 0.2));
        r.SetClearDepth(true, 1.0f);

        Mesh cube;

        {
            using namespace bgfx;

            layout
                .begin()
                .add(Attrib::Position,  3, AttribType::Float)
                .add(Attrib::Color0,    4, AttribType::Uint8, true)
                .end();

            state->vb = createVertexBuffer(
                makeRef(CubeVertices, sizeof(CubeVertices)),
                layout
            );

            state->ib = r.CreateIndexBuffer(&CubeTriangles, sizeof(CubeTriangles));

            state->program = loadProgram("vs_cubes", "fs_cubes");
        }
    }

    void Forward::RenderFrame(Render& r)
    {
        using namespace hlslpp;
        
        // Set view and proj matrices
        {
            float3 at = float3(0, 0, 0);
            float3 eye = float3(0, 0, -35);
            float4x4 view = float4x4::look_at(eye, at, float3(0, 1, 0));

            frustum f = frustum::field_of_view_y(radians(float1(60.0f)), r.GetAspectRatio(), 0.1f, 100.f);
            float4x4 proj = float4x4::perspective(projection(f, zclip::zero));

            r.SetViewTransform(view, proj);
        }

        // Lazy time
        static float time = 0;
        time += 0.01;

        for (int yy = 0; yy < 11; yy++) {
            for(int xx = 0; xx < 11; xx++) {

                // Set object transform
                {
                    // NOTE: bx::mtxRotateXY is right-handed, HLSL++ is set to left-handed,
                    // so we flip the angle signs here to do a right-handed transform.
                    // (Note also that bx::mtxLookAt and bx::mtxProj are left-handed by default)

                    float4x4 mtx = float4x4::rotation_y(-(time+yy*0.37f));
                    mtx = mul(float4x4::rotation_x(-(time + xx*0.21f)), mtx);
                    mtx._m30 = -15.0f + float(xx)*3.0f;
                    mtx._m31 = -15.0f + float(yy)*3.0f;
                    mtx._m32 = 0.0f;
                    r.SetTransform(mtx);
                }

                bgfx::setVertexBuffer(0, state->vb);
                r.SetIndexBuffer(state->ib);
                bgfx::setState(
                    BGFX_STATE_WRITE_RGB
                    | BGFX_STATE_WRITE_Z
                    | BGFX_STATE_DEPTH_TEST_LESS
                    | BGFX_STATE_CULL_CW
                    | BGFX_STATE_MSAA
                );

                bgfx::submit(0, state->program);

            }
        }

    }
}