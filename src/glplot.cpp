#include "glplot.h"


namespace
{
    using attr=lithium::VertexArrayBuffer::AttributeType;

    const char* vertexSrc = R"(#version 330 core
        layout(location = 0) in vec2 aPos;

        uniform mat4 u_projection;

        void main()
        {
            gl_Position = u_projection * vec4(aPos, 0.0, 1.0);
        }
    )";
    
    const char* fragmentSrc = R"(#version 330 core
        out vec4 fragColor;

        uniform sampler2D texture1;

        void main()
        {
            fragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )";

    static std::vector<float> toFloatVector(const std::vector<glm::vec2>& points)
    {
        std::vector<float> result;
        result.reserve(points.size() * 2);
        for (const auto& point : points)
        {
            result.push_back(point.x);
            result.push_back(point.y);
        }
        return result;
    }
}

Plot::Plot(glm::vec2 resolution, size_t maxPoints) : lithium::Object{std::make_shared<lithium::Mesh>(std::vector<attr>{attr::VEC2}, std::vector<GLfloat>{}, std::vector<GLuint>{}), {}},
    _resoultion{resolution}, _orthoCamera{0, resolution.x, 0, resolution.y, -1.0f, 1.0f}
{
    _shaderProgram = std::make_shared<lithium::ShaderProgram>(
        std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(vertexSrc)),
        std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(fragmentSrc)));
    _shaderProgram->setUniform("u_projection", _orthoCamera.projection());

    _mesh->bind();

    GLuint maxVertexSize = static_cast<GLuint>((maxPoints * 2 + 2) * sizeof(GLfloat));
    GLuint maxIndexSize = static_cast<GLuint>((maxPoints * 2 + 2) * sizeof(GLuint));

    _mesh->vertexArrayBuffer(0)->allocate(maxVertexSize, GL_DYNAMIC_DRAW);
    _mesh->elementArrayBuffer()->allocate(maxIndexSize, GL_DYNAMIC_DRAW);
}

Plot::~Plot() noexcept
{

}

void Plot::render()
{
    lithium::Object::render(_shaderProgram.get());
}

void Plot::addPoint(const glm::vec2& point)
{
    // linesegment with width and direction based on previous point
    auto p0 = _points.empty() ? glm::vec2{0.0f, 0.0f} : _points.back();

    std::vector<glm::vec2> lineSegmentVertices = {
        point + glm::vec2(-_lineWidthHalf, +_lineWidthHalf),
        point + glm::vec2(-_lineWidthHalf, -_lineWidthHalf),
        point + glm::vec2(+_lineWidthHalf, -_lineWidthHalf),
        point + glm::vec2(+_lineWidthHalf, +_lineWidthHalf)
    };
    GLuint n = static_cast<GLuint>(_points.size() * 4);
    std::vector<GLuint> lineSegmentIndices = {
        0 + n, 1 + n, 2 + n, // first triangle
        0 + n, 2 + n, 3 + n  // second triangle
    };
    _mesh->bind();
    _mesh->vertexArrayBuffer(0)->appendSubData(toFloatVector(lineSegmentVertices));
    _mesh->elementArrayBuffer()->appendSubData(lineSegmentIndices);
    _points.push_back(point);
}
