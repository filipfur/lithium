#pragma once

#include <memory>
#include "glmesh.h"

namespace lithium
{
    struct MeshVertex
    {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 tex;
        glm::vec3 tangent;
        glm::vec3 bitangent;
    };

    void compute_tangents_lengyel(MeshVertex* pVertices, GLuint kVertices, const GLuint* pIndices, GLuint kIndices);
    std::shared_ptr<lithium::Mesh> tinyobjloader_load(const char * inputfile,
        const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
        glm::vec2 uvScale=glm::vec2{1.0f, 1.0f},
        std::vector<glm::vec3>* vertexPositions=nullptr);

    void tinyobjloader_load_many(const char* inputfile,
        const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
        int numFiles,
        std::vector<std::shared_ptr<lithium::Mesh>>& meshes);
}