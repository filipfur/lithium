#pragma once
#include "glshader.h"
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glelement.h"

namespace mygl
{
	class ShaderProgram : public mygl::Element
	{
	public:
		ShaderProgram(const ShaderProgram& other);

		ShaderProgram(std::string&& vertexShaderFile, std::string&& fragmentShaderFile);

		void use();

		virtual void bind() override
		{
			use();
		}

		virtual void unbind() override
		{

		}

		GLuint loadUniform(const std::string& name);

		void setUniform(const std::string& name, float f)
		{
			glUniform1f(loadUniform(name), f);
		}

		void setUniform(const std::string& name, int i)
		{
			glUniform1i(loadUniform(name), i);
		}

		void setUniform(const std::string& name, const glm::vec3& vector)
		{
			glUniform3fv(loadUniform(name), 1, glm::value_ptr(vector));
		}

		void setUniform(const std::string& name, const glm::ivec3& vector)
		{
			glUniform3iv(loadUniform(name), 1, glm::value_ptr(vector));
		}

		void setUniform(const std::string& name, const glm::vec2& vector)
		{
			glUniform2fv(loadUniform(name), 1, glm::value_ptr(vector));
		}

		void setUniform(const std::string& name, const glm::vec4& vector)
		{
			glUniform4fv(loadUniform(name), 1, glm::value_ptr(vector));
		}

		void setUniform(const std::string& name, const glm::mat4& matrix)
		{
			glUniformMatrix4fv(loadUniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
		}

		void setResolution(const glm::vec2& resolution)
		{
			setUniform("u_resolution", resolution);
		}

		void setTime(float time)
		{
			setUniform("u_time", time);
		}

		void bindUniformBlock(const char* name, GLuint layout)
		{
			GLuint index = glGetUniformBlockIndex(_id, name);
			glUniformBlockBinding(_id, index, layout);
		}

		static unsigned int countBinds()
		{
			unsigned int binds = _bindCount;
			_bindCount = 0;
			return binds;
		}

		~ShaderProgram() noexcept
		{
			glDeleteProgram(_id);
		}

	private:
		Shader<GL_VERTEX_SHADER> _vertexShader;
		Shader<GL_FRAGMENT_SHADER> _fragmentShader;
		std::map<std::string, GLuint> _uniforms;
		static unsigned int _bindCount;
		static mygl::ShaderProgram* _inUse;
	};
}
