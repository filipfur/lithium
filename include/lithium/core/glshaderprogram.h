#pragma once
#include "glshader.h"
#include <map>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glelement.h"

namespace lithium
{
	class ShaderProgram : public lithium::Element
	{
	public:
		ShaderProgram(const ShaderProgram& other);

		ShaderProgram(const std::string& computeShaderFile);

		ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

		ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& geometryShaderFile);

		ShaderProgram(std::shared_ptr<Shader<GL_VERTEX_SHADER>> vertexShader,
			std::shared_ptr<Shader<GL_FRAGMENT_SHADER>> fragmentShader,
			std::shared_ptr<Shader<GL_GEOMETRY_SHADER>> geometryShader=nullptr);

		ShaderProgram(std::shared_ptr<Shader<GL_COMPUTE_SHADER>> computeShader);

		void use();

		void link();
		
		virtual void bind() override
		{
			use();
		}

		virtual void unbind() override
		{

		}

		template <GLuint T>
		bool recompile(std::shared_ptr<lithium::Shader<T>> shader)
		{
			bool compiledShader = false;
			if(shader && !shader->valid())
			{
				shader->compile();
				compiledShader = true;
			}
			return compiledShader;
		}

		void dispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);

		GLuint loadUniform(const std::string& name);

		void setUniform(const std::string& name, float f)
		{
			glUniform1f(loadUniform(name), f);
		}

		void setUniform(const std::string& name, int i)
		{
			glUniform1i(loadUniform(name), i);
			_intCache.emplace(name, i);
		}

		void setUniform(const std::string& name, const glm::vec2& vector, bool cache=false)
		{
			glUniform2fv(loadUniform(name), 1, glm::value_ptr(vector));
			if(cache)
			{
				_vec2Cache.emplace(name, vector);
			}
		}

		void setUniform(const std::string& name, const glm::ivec2& vector)
		{
			glUniform2iv(loadUniform(name), 1, glm::value_ptr(vector));
		}

		void setUniform(const std::string& name, const glm::vec3& vector, bool cache=false)
		{
			glUniform3fv(loadUniform(name), 1, glm::value_ptr(vector));
			if(cache)
			{
				_vec3Cache.emplace(name, vector);
			}
		}

		void setUniform(const std::string& name, const glm::ivec3& vector)
		{
			glUniform3iv(loadUniform(name), 1, glm::value_ptr(vector));
		}

		void setUniform(const std::string& name, const glm::vec4& vector, bool cache=false)
		{
			glUniform4fv(loadUniform(name), 1, glm::value_ptr(vector));
			if(cache)
			{
				_vec4Cache.emplace(name, vector);
			}
		}

		void setUniform(const std::string& name, const glm::mat3& matrix, bool cache=false)
		{
			glUniformMatrix3fv(loadUniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
			if(cache)
			{
				_mat3Cache.emplace(name, matrix);
			}
		}

		void setUniform(const std::string& name, const glm::mat4& matrix, bool cache=false)
		{
			glUniformMatrix4fv(loadUniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
			if(cache)
			{
				_mat4Cache.emplace(name, matrix);
			}
		}

		void setUniform(const std::string& name, const std::vector<glm::vec3>& vectors)
		{
			for(auto i = 0; i < vectors.size(); ++i)
			{
				setUniform(name + "[" + std::to_string(i) + "]", vectors.at(i));
			}
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

		bool checkStatus();

		static const GLuint INVALID_LOCATION{0xffffffff};

	private:
		std::map<std::string,int> _intCache;
		std::map<std::string,glm::mat3> _mat3Cache;
		std::map<std::string,glm::mat4> _mat4Cache;
		std::map<std::string,glm::vec4> _vec4Cache;
		std::map<std::string,glm::vec3> _vec3Cache;
		std::map<std::string,glm::vec2> _vec2Cache;
		std::shared_ptr<Shader<GL_VERTEX_SHADER>> _vertexShader{nullptr};
		std::shared_ptr<Shader<GL_FRAGMENT_SHADER>> _fragmentShader{nullptr};
		std::shared_ptr<Shader<GL_GEOMETRY_SHADER>> _geometryShader{nullptr};
		std::shared_ptr<Shader<GL_COMPUTE_SHADER>> _computeShader{nullptr};
		std::map<std::string, GLuint> _uniforms;
		static unsigned int _bindCount;
		static lithium::ShaderProgram* _inUse;
	};
}
