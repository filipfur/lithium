#pragma once

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <iterator>
#include <glad/glad.h>
#include <vector>
#include <regex>
#include <set>

#include "glfilewatch.h"

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
			if(fileName.size() > 0)
			{
				_watch = lithium::FileWatch::start(
					_fileName, 
					[this](const std::filesystem::path& path) {
						std::cout << "Shader updated: " << _fileName << std::endl;
						_valid = false;		
					}
				);
			}
		}

		static lithium::Shader<T>* fromFile(const std::string& fileName)
		{
			return new Shader("", fileName);

		}

		static lithium::Shader<T>* fromSource(const std::string& source)
		{
			return new Shader(source, "");

		}

		static void checkExpanded(const std::string& fileName, const std::string& src)
		{
// MSVC doesn't support std::regex_constants::multiline, but has multiline regex by default.
#if _MSC_VER
			static std::regex keywordRegex{"^(in|out|uniform|layout|#version)\\b"};
#else
			static std::regex keywordRegex{"^(in|out|uniform|layout|#version)\\b", std::regex_constants::multiline};
#endif
			std::sregex_iterator begin(src.begin(), src.end(), keywordRegex), end;
			std::set<std::string> keywordSet;
			std::for_each(begin, end, [&keywordSet](const std::smatch& m) {
				keywordSet.emplace(m.str());
			});
			if(keywordSet.size() > 0)
			{
				for(auto& keyword : keywordSet)
				{
					std::cerr << "Error: Illegal keyword when expanding '" << fileName << "': " << keyword << std::endl;
				}
				exit(1);
			}
		}

		static std::string expandSource(const std::filesystem::path& ppath, const std::string& src)
		{
			std::stringstream ss;
			std::string line;
			std::istringstream iss(src);
			static const char* tag = "#include";
			while(std::getline(iss, line))
			{
				if(line.find(tag) != std::string::npos)
				{
					size_t a = strlen(tag);
					while(std::isspace(line[a]))
					{
						++a;
					}
					std::string includeFile = line.substr(a, line.length() - a);
					auto includeFilePath = ppath / includeFile;
					size_t s0 = includeFile.find_first_of('<');
					if(s0 != std::string::npos)
					{
						size_t s1 = includeFile.find_last_of('>');
						static std::filesystem::path shaderStdLibPath{"lithium/glsl"};
						includeFile = includeFile.substr(s0 + 1, s1 - s0 - 1);
						includeFilePath = shaderStdLibPath / includeFile;
					}
					std::string includeSource = readFile(includeFilePath.string());
					checkExpanded(includeFile, includeSource);
					ss << expandSource(includeFilePath.parent_path(), includeSource);
				}
				else
				{
					ss << line << std::endl;
				}
			}
			return ss.str();
		}

		void compile()
		{
			if(_fileName.size() > 0)
			{
				_source = readFile(_fileName);
				std::filesystem::path filePath{_fileName};
				_source = expandSource(filePath.parent_path(), _source);
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
			_watch = nullptr;
		}

		bool valid() const
		{
			return _valid;
		}

		std::string fileName() const
		{
			return _fileName;
		}

		static std::string readFile(const std::string& fileName)
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

	private:
		GLuint _id;
		bool _valid{true};
		std::string _source;
		std::string _fileName;
		std::shared_ptr<lithium::FileWatch> _watch{nullptr};
	};

	using VertexShader = lithium::Shader<GL_VERTEX_SHADER>;
	using FragmentShader = lithium::Shader<GL_FRAGMENT_SHADER>;
	using GeometryShader = lithium::Shader<GL_GEOMETRY_SHADER>;
	using ComputeShader = lithium::Shader<GL_COMPUTE_SHADER>;
}
