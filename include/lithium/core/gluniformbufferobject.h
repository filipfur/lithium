#pragma once

#include "glbuffer.h"
#include "glshaderprogram.h"

namespace lithium
{
    class UniformBufferObject : public Buffer
    {
    public:
        UniformBufferObject(GLuint size, const std::string& label, GLuint layout) : Buffer{GL_UNIFORM_BUFFER}, _label{label}, _layout{layout}
        {
            bind();
            glBufferData(type(), size, 0, GL_STATIC_DRAW);
            unbind();
        }

        void bindBufferBase(const std::vector<lithium::ShaderProgram*>& shaders)
        {
            bind();
            for(auto && sp : shaders)
            {
	            sp->bindUniformBlock(_label.c_str(), _layout);
            }
            glBindBufferBase(GL_UNIFORM_BUFFER, _layout, id());
            unbind();
        }

        void bufferSubData(GLuint offset, const glm::mat4& matrix)
        {
            bind();
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(matrix));
            unbind();
        }

        void bufferSubData(GLuint offset, const glm::vec3& vec)
        {
            bind();
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(vec));
            unbind();
        }

        void bufferSubData(GLuint offset, const glm::vec4& vec)
        {
            bind();
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec4), glm::value_ptr(vec));
            unbind();
        }

        void bufferSubData(GLuint offset, float f)
        {
            bind();
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(float), &f);
            unbind();
        }

        virtual ~UniformBufferObject() noexcept
        {
            
        }

    private:
        const std::string _label;
        const GLuint _layout;
    };
}
