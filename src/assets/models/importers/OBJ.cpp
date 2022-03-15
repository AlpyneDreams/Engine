#include "common/Common.h"

#include <filesystem>
#include <functional>
#include <unordered_map>

#include "math/Math.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "OBJ.h"

struct Vertex {
    Vector3 pos;
    Vector3 color;
    Vector2 uv;

    bool operator==(const Vertex& other) const {
        using namespace hlslpp;
        return any(pos == other.pos) && any(color == other.color) && any(uv == other.uv);
    }
};

// ugh!!!
namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<Vector3>()(vertex.pos) ^ (hash<Vector3>()(vertex.color) << 1)) >> 1) ^ (hash<Vector2>()(vertex.uv) << 1);
        }
    };
}

namespace engine
{
    Mesh* LoadMeshOBJ(const char* path)
    {
        using namespace tinyobj;

        ObjReaderConfig config;
        config.mtl_search_path = std::filesystem::path(path).parent_path();

        ObjReader reader;

        if (!reader.ParseFromFile(path, config)) {
            std::fprintf(stderr, "[OBJ] Error loading '%s'\n", path);
            if (!reader.Error().empty()) {
                std::fprintf(stderr, "[OBJ] Error: %s\n", reader.Error().c_str());
            }
        }

        if (!reader.Warning().empty()) {
            std::fprintf(stderr, "[OBJ] Warning: %s\n", reader.Warning().c_str());
        }

        auto& attribs = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        Mesh* mesh = new Mesh();
        mesh->layout.Add<float>(3, VertexAttribute::Position);
        mesh->layout.Add<float>(3, VertexAttribute::Color);
        mesh->layout.Add<float>(2, VertexAttribute::TexCoord);

        bool hasUV = attribs.texcoords.size() > 0;

        std::unordered_map<Vertex, uint32> uniqueVertices {};

        for (const auto& shape : shapes) {

            auto& group = mesh->AddGroup();
            std::vector<Vertex>* vertices = new std::vector<Vertex>();
            std::vector<uint32>* indices = new std::vector<uint32>();

            for (const auto& index : shape.mesh.indices) {
                Vertex vertex;

                vertex.pos = {
                    attribs.vertices[3 * index.vertex_index + 0],
                    attribs.vertices[3 * index.vertex_index + 1],
                    attribs.vertices[3 * index.vertex_index + 2],
                };

                if (index.normal_index != -1) {
                    vertex.color = {
                        attribs.normals[3 * index.normal_index + 0],
                        attribs.normals[3 * index.normal_index + 1],
                        attribs.normals[3 * index.normal_index + 2],
                    };
                } else {
                    vertex.color = {1.f, 0.f, 0.f};
                }

                if (hasUV) {
                    vertex.uv = {
                        attribs.texcoords[2 * index.texcoord_index + 0],
                        attribs.texcoords[2 * index.texcoord_index + 1],
                    };
                } else {
                    vertex.uv = {0.f, 0.f};
                }

                if (uniqueVertices.count(vertex) == 0) {
                    uniqueVertices[vertex] = vertices->size();
                    vertices->push_back(vertex);
                }
                
                indices->push_back(uniqueVertices[vertex]);
            }
            
            group.vertices.vertexCount = vertices->size();
            group.vertices.vertices = vertices->data();

            group.indices.indexCount = indices->size();
            group.indices.indices = indices->data();
        }

        return mesh;
    }
}
