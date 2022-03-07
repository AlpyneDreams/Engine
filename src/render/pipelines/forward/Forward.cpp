#include "Forward.h"
#include "common/Common.h"
#include "common/Filesystem.h"
#include "math/Math.h"
#include "core/Mesh.h"

#include <raylib.h>

#include <cstdio>

namespace fs = engine::fs;

namespace engine::render
{
    using Forward = ForwardRenderPipeline;

    struct ForwardState
    {
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

    void Forward::Init(Render& r)
    {
        r.SetClearColor(true, Colorf(0.2, 0.2, 0.2));
        r.SetClearDepth(true, 1.0f);
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

        Camera3D camera = { 0 };
        camera.position = (Vector3){ 0.0f, 0.f, -35.f };  // Camera position
        camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
        camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
        camera.fovy = 60.0f;                                // Camera field-of-view Y
        camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

        Vector3 cubePos = Vector3(0.f, 0.f, 0.f);

        BeginMode3D(camera);

        //DrawCube(cubePos, 2.f, 2.f, 2.f, RED);
        //DrawCubeWires(cubePos, 2.f, 2.f, 2.f, MAROON);

        DrawGrid(10, 1.0f);
            
        // Lazy time
        static float time = 0;
        time += 0.01;

        for (int yy = 0; yy < 11; yy++) {
            for(int xx = 0; xx < 11; xx++) {

                DrawCube(Vector3(-15.0f + float(xx)*3.0f, -15.0f + float(yy)*3.0f, 0.f), 2.f, 2.f, 2.f, RED);

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

                /*bgfx::setVertexBuffer(0, state->vb);
                bgfx::setIndexBuffer(state->ib);
                bgfx::setState(
                    BGFX_STATE_WRITE_RGB
                    | BGFX_STATE_WRITE_Z
                    | BGFX_STATE_DEPTH_TEST_LESS
                    | BGFX_STATE_CULL_CW
                    | BGFX_STATE_MSAA
                );

                bgfx::submit(0, state->program);*/

            }
        }

        EndMode3D();

        DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);
        DrawFPS(10, 10);

    }
}