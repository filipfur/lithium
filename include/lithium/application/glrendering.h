#pragma once

#include "glshaderprogram.h"
#include "globject.h"

namespace lithium
{
    class Rendering
    {
    public:
        Rendering(lithium::ShaderProgram* shaderProgram, const std::vector<lithium::Object*>& objects)
            : _shaderProgram{shaderProgram}, _objects{objects}
        {

        }

        virtual ~Rendering() noexcept
        {

        }

        void render()
        {
            for(lithium::Object* object : _objects)
            {
                object->shade(_shaderProgram);
                object->draw();
            }
        }

    private:
        lithium::ShaderProgram* _shaderProgram{nullptr};
        std::vector<lithium::Object*> _objects{nullptr};
    };
}