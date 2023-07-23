#include "glcube.h"

namespace
{
    using attr = lithium::VertexArrayBuffer::AttributeType;
    static constexpr attr POSITION{attr::VEC3};
    static constexpr attr NORMALS{attr::VEC3};
    static constexpr attr UV{attr::VEC2};
    static constexpr attr XY{attr::VEC2};
}

lithium::Mesh* lithium::Cube(const glm::vec3& scale, const glm::vec2& uvScale)
{
    std::vector<GLuint> indices = {
        0, 1, 2, 2, 3, 0, // front
        4, 5, 6, 6, 7, 4, // back
        8, 9, 10, 10, 11, 8, // top
        12, 13, 14, 14, 15, 12, // bottom
        16, 17, 18, 18, 19, 16, // left
        20, 21, 22, 22, 23, 20 // right
    };

    //vertices based on scale and uv scale.  POSITION NORMALS UV
    std::vector<GLfloat> vertices = {
        // front
        -scale.x, -scale.y, scale.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -scale.x, scale.y, scale.z, 0.0f, 0.0f, 1.0f, 0.0f, uvScale.y,
        scale.x, scale.y, scale.z, 0.0f, 0.0f, 1.0f, uvScale.x, uvScale.y,
        scale.x, -scale.y, scale.z, 0.0f, 0.0f, 1.0f, uvScale.x, 0.0f,
        // back
        -scale.x, -scale.y, -scale.z, 0.0f, 0.0f, -1.0f, uvScale.x, 0.0f,
        -scale.x, scale.y, -scale.z, 0.0f, 0.0f, -1.0f, uvScale.x, uvScale.y,
        scale.x, scale.y, -scale.z, 0.0f, 0.0f, -1.0f, 0.0f, uvScale.y,
        scale.x, -scale.y, -scale.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        // top
        -scale.x, scale.y, scale.z, 0.0f, 1.0f, 0.0f, 0.0f, uvScale.y,
        -scale.x, scale.y, -scale.z, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        scale.x, scale.y, -scale.z, 0.0f, 1.0f, 0.0f, uvScale.x, 0.0f,
        scale.x, scale.y, scale.z, 0.0f, 1.0f, 0.0f, uvScale.x, uvScale.y,
        // bottom
        -scale.x, -scale.y, scale.z, 0.0f, -1.0f, 0.0f, uvScale.x, uvScale.y,
        -scale.x, -scale.y, -scale.z, 0.0f, -1.0f, 0.0f, uvScale.x, 0.0f,
        scale.x, -scale.y, -scale.z, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        scale.x, -scale.y, scale.z, 0.0f, -1.0f, 0.0f, 0.0f, uvScale.y,
        // left
        -scale.x, -scale.y, -scale.z, -1.0f, 0.0f, 0.0f, uvScale.x, uvScale.y,
        -scale.x, scale.y, -scale.z, -1.0f, 0.0f, 0.0f, uvScale.x, 0.0f,
        -scale.x, scale.y, scale.z, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -scale.x, -scale.y, scale.z, -1.0f, 0.0f, 0.0f, 0.0f, uvScale.y,
        // right
        scale.x, -scale.y, -scale.z, 1.0f, 0.0f, 0.0f, uvScale.x, uvScale.y,
        scale.x, scale.y, -scale.z, 1.0f, 0.0f, 0.0f, uvScale.x, 0.0f,
        scale.x, scale.y, scale.z, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        scale.x, -scale.y, scale.z, 1.0f, 0.0f, 0.0f, 0.0f, uvScale.y
    };

    return new Mesh({POSITION, NORMALS, UV},
        vertices, indices);
}

lithium::Mesh* lithium::InvCube(float s)
{
    std::vector<GLfloat> vertices = {
        // positions          
        -s,  s, -s,
        -s, -s, -s,
        s, -s, -s,
        s, -s, -s,
        s,  s, -s,
        -s,  s, -s,

        -s, -s,  s,
        -s, -s, -s,
        -s,  s, -s,
        -s,  s, -s,
        -s,  s,  s,
        -s, -s,  s,

        s, -s, -s,
        s, -s,  s,
        s,  s,  s,
        s,  s,  s,
        s,  s, -s,
        s, -s, -s,

        -s, -s,  s,
        -s,  s,  s,
        s,  s,  s,
        s,  s,  s,
        s, -s,  s,
        -s, -s,  s,

        -s,  s, -s,
        s,  s, -s,
        s,  s,  s,
        s,  s,  s,
        -s,  s,  s,
        -s,  s, -s,

        -s, -s, -s,
        -s, -s,  s,
        s, -s, -s,
        s, -s, -s,
        -s, -s,  s,
        s, -s,  s
    };

    static const std::vector<GLfloat> skyboxVertices = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};
    return new lithium::Mesh({POSITION}, skyboxVertices);
}