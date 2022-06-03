#include "Forward.h"
#include "common/Common.h"
#include "common/Filesystem.h"
#include "math/Math.h"
#include "platform/Platform.h"

#include "engine/Time.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/string.h>

#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <cstdio>

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
        // Set shader
        r.SetShader(shader);
        
        // Set view and proj matrices
        {
            Vector3 at = Vector3(0, 0, 0);
            Vector3 eye = Vector3(0, 0, -35);
            Matrix4x4 view = glm::lookAtLH(eye, at, Vector3(0, 1, 0));

            
            Matrix4x4 proj = glm::perspectiveLH_ZO(glm::radians(60.f), r.GetAspectRatio(), 0.1f, 100.f);

            r.SetViewTransform(view, proj);
        }

        float time = Time.fixed.time;

        for (int yy = 0; yy < 11; yy++) {
            for(int xx = 0; xx < 11; xx++) {

                // Set object transform
                {
                    Matrix4x4 mtx = glm::translate(glm::identity<Matrix4x4>(), Vector3(-15 + xx*3, -15 + yy*3, 0));
                    mtx *= glm::eulerAngleYX(-(time + yy*0.37f), -(time + xx*0.21f));
                    
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