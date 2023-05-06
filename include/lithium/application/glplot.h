#pragma once

#include "globject.h"
#include "glorthographiccamera.h"


class Plot : public lithium::Object
{
public:
    Plot(glm::vec2 resolution, size_t maxPoints=1000);

    virtual ~Plot() noexcept;
    

    void render();

    void addPoint(const glm::vec2& point);

    void setLineWidth(float width)
    {
        _lineWidth = width;
        _lineWidthHalf = width * 0.5f;
    }

private:
    glm::vec2 _resoultion;
    lithium::OrthographicCamera _orthoCamera;
    std::shared_ptr<lithium::ShaderProgram> _shaderProgram;
    std::vector<glm::vec2> _points;
    float _lineWidth{1.0f};
    float _lineWidthHalf{0.5f};
};