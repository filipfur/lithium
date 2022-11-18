#pragma once

namespace lithium
{
    class ShaderProgram;

    class Renderable
    {
    public:
        virtual void shade(lithium::ShaderProgram* shaderProgram) = 0;
        virtual void draw() = 0;
    };
}