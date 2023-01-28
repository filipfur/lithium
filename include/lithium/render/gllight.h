#pragma once

#include "globject.h"
#include "glshaderprogram.h"

namespace lithium
{
    class Light : public Object
    {
    public:
        Light(lithium::Mesh* mesh);
        Light(const Light& other);
        Light(const Object& other);
        virtual ~Light() noexcept;

        

        void attachShader(lithium::ShaderProgram* shaderProgram)
        {
            _attachedShaders.push_back(shaderProgram);
        }

    protected:
        virtual void updateModel() override
        {
            lithium::Object::updateModel();
            for(auto shader : _attachedShaders)
            {
                shader->use();
                shader->setUniform("u_light_position", position());
                shader->setUniform("u_light_color", color());
            }    
        }

    private:
        std::vector<lithium::ShaderProgram*> _attachedShaders;
    };
}