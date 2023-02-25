#pragma once

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <vector>

#ifdef _WIN32
#include <filesystem>
#include "FileWatch.hpp"
#endif

namespace lithium
{
	template <GLuint T>
	class Shader
	{
	public:

		Shader(const Shader& other) : _id{other._id}, _fileName{other._fileName}
		{

		}

		Shader(const std::string& source, const std::string& fileName) : _id{}, _source { source }, _fileName{fileName}
		{
			_id = glCreateShader(T);
			compile();
			#ifdef _WIN32
			_watch = std::make_shared<filewatch::FileWatch<std::string>>(
				"./" + _fileName, 
				[this](const std::string& path, const filewatch::Event change_type) {
					std::cout << "Shader updated: " << _fileName << std::endl;
					_valid = false;		
				}
			);
			#endif
		}

		static lithium::Shader<T>* fromFile(const std::string& fileName)
		{
			return new Shader("", fileName);

		}

		static lithium::Shader<T>* fromSource(const std::string& source)
		{
			return new Shader(source, "");

		}

		void compile()
		{
			if(_fileName.size() > 0)
			{
				_source = readFile(_fileName);
			}
			const char* src = _source.c_str();
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
				//std::cout << "'" << _fileName << "' compiled sucessfully." << std::endl;
			}
			_valid = true;
			_source = "";
		}

		GLuint id() const
		{
			return _id;
		}

		~Shader() noexcept
		{
			glDeleteShader(_id);
			#ifdef _WIN32
			delete _watch;
			#endif
		}

		bool valid() const
		{
			return _valid;
		}

		std::string fileName() const
		{
			return _fileName;
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
		bool _valid{true};
		std::string _source;
		std::string _fileName;
		#ifdef _WIN32
		std::shared_ptr<filewatch::FileWatch<std::string>> _watch{nullptr};
		#endif
	};
}
