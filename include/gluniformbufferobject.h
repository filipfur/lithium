#pragma once

#include "glbuffer.h"
#include "glshaderprogram.h"

namespace lithium
{
    class UniformBufferObject : public Buffer<GLfloat, GL_UNIFORM_BUFFER>
    {
    public:
        UniformBufferObject(GLuint size, const std::string& label, GLuint layout) : Buffer{size}, _label{label}, _layout{layout}
        {

        }

        void bindBufferBase(const std::vector<lithium::ShaderProgram*>& shaders) const
        {
            for(auto && sp : shaders)
            {
	            sp->bindUniformBlock(_label.c_str(), _layout);
            }
            glBindBufferBase(GL_UNIFORM_BUFFER, _layout, id());
        }

        void bufferSubData(GLuint offset, const glm::mat4& matrix)
        {
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4), glm::value_ptr(matrix));
        }

        void bufferSubData(GLuint offset, const glm::vec3& vec)
        {
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::vec3), glm::value_ptr(vec));
        }

        void bufferSubData(GLuint offset, GLfloat f)
        {
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(GLfloat), &f);
        }

        virtual ~UniformBufferObject() noexcept
        {
            
        }

    private:
        const std::string _label;
        const GLuint _layout;
    };
}
