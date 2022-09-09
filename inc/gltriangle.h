#pragma once

#include "glshaderprogram.h"
#include "gltexture.h"
#include "glcamera.h"

namespace mygl
{
    class Triangle
    {
    public:
        Triangle(mygl::ShaderProgram* shaderProgram);

        void setTexture(mygl::Texture* texture)
        {
            _texture = texture;
        }

        void draw(Camera* camera);

        virtual ~Triangle() noexcept;
    private:

        GLuint _VAO;
        GLuint _VBO;
        mygl::ShaderProgram* _shaderProgram;
        mygl::Texture* _texture;

    };
}