#include "glplane.h"

namespace
{
    using attr = lithium::VertexArrayBuffer::AttributeType;
    static constexpr attr POSITION{attr::VEC3};
    static constexpr attr NORMALS{attr::VEC3};
    static constexpr attr UV{attr::VEC2};
    static constexpr attr XY{attr::VEC2};
    static const std::vector<GLfloat> vertices2D = {
        -1.0, -1.0,     0.0f, 0.0f,
        -1.0,  1.0,     0.0f, 1.0, 
        1.0,  1.0,      1.0, 1.0,  
        1.0, -1.0,      1.0, 0.0f
    };
    static const std::vector<GLfloat> vertices3D = {
        -1.0, -1.0, 0.0f, 	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
        -1.0,  1.0, 0.0f, 	0.0f, 1.0f, 0.0f,	0.0f, 1.0, 
        1.0,  1.0, 0.0f,	0.0f, 1.0f, 0.0f,	1.0, 1.0,  
        1.0, -1.0, 0.0f, 	0.0f, 1.0f, 0.0f,	1.0, 0.0f
    };

    static const std::vector<GLuint> indices = {
        0, 2, 1,
        0, 3, 2
    };
}

lithium::Mesh* lithium::Plane2D(const glm::vec2& scale, const glm::vec2& uvScale)
{
    std::vector<GLfloat> vertices2D = {
        -scale.x, -scale.y,    +0.0f, +0.0f,
        -scale.x, +scale.y,    +0.0f, +uvScale.y, 
        +scale.x, +scale.y,    +uvScale.x, +uvScale.y,  
        +scale.x, -scale.y,    +uvScale.x, +0.0f
    };
    return new lithium::Mesh({XY, UV}, vertices2D, indices);
}

lithium::Mesh* lithium::Plane3D(const glm::vec2& scale, const glm::vec2& uvScale)
{
    std::vector<GLfloat> vertices3D = {
        -scale.x, -scale.y, 0.0f, 	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
        -scale.x, +scale.y, 0.0f, 	0.0f, 1.0f, 0.0f,	0.0f, uvScale.y, 
        +scale.x, +scale.y, 0.0f,	0.0f, 1.0f, 0.0f,	uvScale.x, uvScale.y,  
        +scale.x, -scale.y, 0.0f, 	0.0f, 1.0f, 0.0f,	uvScale.x, 0.0f
    };
    return new lithium::Mesh({POSITION, NORMALS, UV}, vertices3D, indices);
}