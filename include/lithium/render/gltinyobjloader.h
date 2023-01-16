#pragma once

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
    lithium::Mesh* tinyobjloader_load(const char * inputfile,
        const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
        glm::vec2 uvScale=glm::vec2{1.0f, 1.0f});

    void tinyobjloader_load_many(const char* inputfile,
        const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
        int numFiles,
        std::vector<lithium::Mesh*>& meshes);
}