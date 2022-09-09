#pragma once

#include "globject.h"

#include "glshaderprogram.h"

namespace mygl
{
    class Light : public Object
    {
    public:
        Light(mygl::Mesh* mesh, mygl::ShaderProgram* shaderProgram);
        Light(const Light& other);
        Light(const Object& other);
        virtual ~Light() noexcept;

        

        void attachShader(mygl::ShaderProgram* shaderProgram)
        {
            _attachedShaders.push_back(shaderProgram);
        }

    protected:
        virtual void updateModel() override
        {
            mygl::Object::updateModel();
            for(auto shader : _attachedShaders)
            {
                shader->use();
                shader->setUniform("u_light_position", position());
                shader->setUniform("u_light_color", color());
            }    
        }

    private:
        std::vector<mygl::ShaderProgram*> _attachedShaders;
    };
}