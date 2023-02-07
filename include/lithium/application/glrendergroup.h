#pragma once

#include "glrenderable.h"

namespace lithium
{
    class RenderGroup
    {
    public:
        RenderGroup(const std::vector<lithium::Renderable*>& objects)
            : _objects{objects}
        {

        }

        virtual ~RenderGroup() noexcept
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
        std::vector<lithium::Renderable*> _objects{nullptr};
    };
}