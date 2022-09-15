#pragma once

#include "glshaderprogram.h"
#include "gltexture.h"
#include "glcamera.h"

namespace lithium
{
    class Triangle
    {
    public:
        Triangle(lithium::ShaderProgram* shaderProgram);

        void setTexture(lithium::Texture* texture)
        {
            _texture = texture;
        }

        void draw(Camera* camera);

        virtual ~Triangle() noexcept;
    private:

        GLuint _VAO;
        GLuint _VBO;
        lithium::ShaderProgram* _shaderProgram;
        lithium::Texture* _texture;

    };
}