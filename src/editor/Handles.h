#pragma once

#include "core/Mesh.h"
#include "editor/Editor.h"
#include "glm/ext/matrix_transform.hpp"
#include "math/Math.h"
#include "render/Render.h"
#include "entity/components/Transform.h"

#include <vector>

namespace engine::editor
{
    inline struct Handles
    {
    private:
        static inline Mesh grid;
        static inline std::vector<vec4> gridVertices;

    public:
        // TODO: Grid major/minor lines. View based fade. Infinite grid
        void DrawGrid(render::Render& r)
        {
            mat4x4 matrix = glm::translate(glm::identity<mat4x4>(), vec3(0, 0.015, 0));
            r.SetTransform(matrix);
            r.SetDepthTest(render::CompareFunc::LessEqual);
            r.SetPolygonMode(render::PolygonMode::Lines);
            r.SetShader(Editor.sh_Grid);
            r.DrawMesh(&grid);
            r.SetPolygonMode(render::PolygonMode::Fill);
            r.SetDepthTest(render::CompareFunc::Less);
        }

        Handles()
        {
            int gridSize = 100;
            int radius = gridSize / 2;
            int gridMajor = 10;
            gridVertices.resize((gridSize + 1) * 4);

            uint i = 0;
            for (int x = -radius; x <= radius; x++, i += 2)
            {
                gridVertices[i] = vec4(x, 0, -radius, x % gridMajor == 0);
                gridVertices[i+1] = vec4(x, 0, +radius, x % gridMajor == 0);
            }

            for (int z = -radius; z <= radius; z++, i += 2)
            {
                gridVertices[i] = vec4(-radius, 0, z, z % gridMajor == 0);
                gridVertices[i+1] = vec4(+radius, 0, z, z % gridMajor == 0);
            }

            auto& g = grid.AddGroup();
            g.vertices.layout.Add<float>(3, VertexAttribute::Position);
            g.vertices.layout.Add<float>(1, VertexAttribute::TexCoord);
            g.vertices.pointer = &gridVertices[0].x; g.vertices.count = gridVertices.size();
        }
    } Handles;
}