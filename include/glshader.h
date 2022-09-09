#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <vector>

namespace mygl
{
	template <GLuint T>
	class Shader
	{
	public:

		Shader(const Shader& other) : _id{other._id}, _fileName{other._fileName}
		{

		}

		Shader(std::string&& fileName) : _id{}, _fileName { std::move(fileName) }
		{
			std::string source = readFile(_fileName);
			const char* src = source.c_str();
			_id = glCreateShader(T);
			glShaderSource(_id, 1, &src, nullptr);
			glCompileShader(_id);
			GLint status = 0;
			glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				GLint length = 0;
				glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length);
				std::vector<GLchar> errorLog(length);
				glGetShaderInfoLog(_id, length, &length, &errorLog[0]);
				std::cout << "'" << _fileName << "' compiled with error:" << std::endl;
				std::copy(errorLog.begin(), errorLog.end(), std::ostream_iterator<GLchar>(std::cout, ""));
				exit(1);
			}
			else
			{
				std::cout << "'" << _fileName << "' compiled sucessfully." << std::endl;
			}
		}

		GLuint id() const
		{
			return _id;
		}

		~Shader() noexcept
		{
			glDeleteShader(_id);
		}

	private:
		std::string readFile(const std::string& fileName)
		{
			std::ifstream ifs{ fileName };
			if(!ifs)
			{
				std::cerr << "Failed to open file: " << fileName << std::endl;
				exit(1);
			}
			std::stringstream buffer;
			buffer << ifs.rdbuf();
			return buffer.str().c_str();
		}

		GLuint _id;
		std::string _fileName;
	};
}
