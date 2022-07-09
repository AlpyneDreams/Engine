#include "Forward.h"
#include "common/Common.h"
#include "common/Filesystem.h"
#include "math/Math.h"
#include "platform/Platform.h"
#include "entity/Entity.h"
#include "entity/components/Transform.h"
#include "entity/components/MeshRenderer.h"

#include "engine/Time.h"
#include "render/RenderContext.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/string.h>

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

    static Entity ent;

    void Forward::Init()
    {
        ent.AddComponent<Transform>();
        MeshRenderer& mr = ent.AddComponent<MeshRenderer>();

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
        
        mr.mesh = &cube;
    }

    void Forward::Render(RenderContext& ctx)
    {
        // Set shader
        r.SetShader(shader);
        
        // Set viewport and matrices
        ctx.SetupCamera();

        ctx.DrawRenderers();
    }
}