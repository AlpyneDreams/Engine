#include "Forward.h"
#include "common/Common.h"
#include "common/Filesystem.h"
#include "math/Math.h"
#include "platform/Platform.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/math.h>
#include <bx/string.h>

#include <cstdio>

namespace fs = engine::fs;

namespace engine::render
{
    using Forward = ForwardRenderPipeline;

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
    

    void Forward::Start()
    {
        r.SetClearColor(true, Color(0.2, 0.2, 0.2));
        r.SetClearDepth(true, 1.0f);

        {
            VertexLayout layout;
            layout
              .Add<float>(3, VertexAttribute::Position)
              .Add<uint8>(4, VertexAttribute::Color, true);
            
            cube = Mesh(layout, CubeVertices, sizeof(CubeVertices), CubeTriangles, sizeof(CubeTriangles));
            r.UploadMesh(&cube);

            shader = r.LoadShader("vs_cubes", "fs_cubes");
        }
    }

    void Forward::Update()
    {
        using namespace hlslpp;

        // Set shader
        r.SetShader(shader);
        
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

                bgfx::setState(
                    BGFX_STATE_WRITE_RGB
                    | BGFX_STATE_WRITE_Z
                    | BGFX_STATE_DEPTH_TEST_LESS
                    | BGFX_STATE_CULL_CW
                    | BGFX_STATE_MSAA
                );
                r.DrawMesh(&cube);

                //r.Submit();

            }
        }

    }
}