#pragma once

#include <iostream>
#include <string>
#include "glelement.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace lithium
{
	template <typename T=unsigned char>
	class Texture : public Element
	{
	public:
		Texture(T* bytes, int width, int height, GLenum type=GL_UNSIGNED_BYTE,
			GLenum internalFormat=GL_RGBA, GLenum colorFormat=GL_RGBA,
			GLenum textureMode=GL_TEXTURE_2D, GLuint samples=4)
			: _bytes{bytes}, _width{width}, _height{height}, _type{type}, _internalFormat{internalFormat},
				_colorFormat{colorFormat}, _textureMode{textureMode}, _samples{samples}
		{
			glGenTextures(1, &_id);
			bind();
			loadBytes(bytes);
			errorCheck();
			setUnpackAlignment();
			setFilter()->setWrap();
			unbind();
		}

		virtual ~Texture() noexcept
		{
			glDeleteTextures(1, &_id);
		}

		static std::shared_ptr<lithium::Texture<T>> Basic(const glm::vec3& color=glm::vec3{1.0f})
		{
			T data[] = {
				static_cast<unsigned char>(255.0f * color.r),
				static_cast<unsigned char>(255.0f * color.g),
				static_cast<unsigned char>(255.0f * color.b)};
            return std::make_shared<lithium::Texture<T>>(data, 1, 1, GL_UNSIGNED_BYTE, GL_RGB, GL_RGB);
		}

		Texture* setUnpackAlignment(GLuint unpackAlignment=4)
		{
			bind();
			glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment);
			unbind();
			return this;
		}

		Texture* setFilter(GLenum min, GLenum mag)
		{
			bind();
			glTexParameteri(_textureMode, GL_TEXTURE_MIN_FILTER, min);
			glTexParameteri(_textureMode, GL_TEXTURE_MAG_FILTER, mag);
			unbind();
			return this;
		}

		Texture* setFilter(GLenum filter=GL_LINEAR) { return setFilter(filter, filter); }

		Texture* setWrap(GLenum wrapS, GLenum wrapT, GLenum wrapR)
		{
			bind();
			glTexParameteri(_textureMode, GL_TEXTURE_WRAP_S, wrapS);
			glTexParameteri(_textureMode, GL_TEXTURE_WRAP_T, wrapT);
			glTexParameteri(_textureMode, GL_TEXTURE_WRAP_R, wrapR);
			unbind();
			return this;
		}

		Texture* setWrap(GLenum wrap=GL_CLAMP_TO_EDGE) { return setWrap(wrap, wrap, wrap); }

		Texture* generateMipmap()
		{
			glGenerateMipmap(_textureMode);
		}

		Texture* setBorderColor(const glm::vec4& borderColor)
		{
			bind();
			glTexParameterfv(_textureMode, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
			unbind();
			return this;
		}

		GLenum textureMode() const
		{
			return _textureMode;
		}

		Texture* bind(GLuint textureUnit)
		{
			activate(textureUnit);
			bind();
			return this;
		}

		int width() const
		{
			return _width;
		}

		int height() const
		{
			return _height;
		}

		static bool activate(GLuint textureUnit)
		{
			if(_active != textureUnit)
			{
				glActiveTexture(textureUnit);
				_active = textureUnit;
				return true;
			}
			return false;
		}

		virtual void unbind() override
		{
			_bound[_active - GL_TEXTURE0] = nullptr;
			glBindTexture(_textureMode, 0);
		}

		static unsigned int countBinds()
		{
			unsigned int binds = _bindCount;
			_bindCount = 0;
			return binds;
		}

		void bindImageTexture(GLuint unit, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
		{
			glBindImageTexture(unit, _id, level, layered, layer, access, format);
		}

		virtual void bind() override
		{
			if(_active == 0)
			{
				activate(GL_TEXTURE0);
			}
			int index = _active - GL_TEXTURE0;
			if(_bound[index] != this)
			{
				glBindTexture(_textureMode, _id);
				_bound[index] = this;
				++_bindCount;
			}
		}
	protected:

		void loadBytes(T* bytes)
		{
			_bytes = bytes;
			switch(_textureMode)
			{
				default:
				case GL_TEXTURE_2D:
					glTexImage2D(_textureMode, 0, _internalFormat, _width, _height, 0, _colorFormat, _type, _bytes);
					break;
				case GL_TEXTURE_2D_MULTISAMPLE:
					glTexImage2DMultisample(_textureMode, _samples, _internalFormat, _width, _height, GL_TRUE);
					break;
				case GL_TEXTURE_CUBE_MAP:
					break;
			}
		}

		void errorCheck()
		{
			GLuint error{glGetError()};
			if(error != GL_NO_ERROR)
			{
				std::string errorString;
				switch(error)
				{
					case GL_INVALID_ENUM:
						errorString = "GL_INVALID_ENUM";
						break;
					case GL_INVALID_VALUE:
						errorString = "GL_INVALID_VALUE";
						break;
					case GL_INVALID_OPERATION:
						errorString = "GL_INVALID_OPERATION";
						break;
					case GL_INVALID_FRAMEBUFFER_OPERATION:
						errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
						break;
					case GL_OUT_OF_MEMORY:
						errorString = "GL_OUT_OF_MEMORY";
						break;
					default:
						errorString = "UNKNOWN";
						break;
				}
				std::cerr << "Error creating texture: " << errorString << std::endl;
			}	
		}

		T* bytes() const
		{
			return _bytes;
		}

		GLenum type() const
		{
			return _type;
		}

		GLenum internalFormat() const
		{
			return _internalFormat;
		}

		GLenum colorFormat() const
		{
			return _colorFormat;
		}

		GLuint samples() const
		{
			return _samples;
		}

		void setWidth(int width)
		{
			_width = width;
		}

		void setHeight(int height)
		{
			_height = height;
		}

		void setColorFormat(GLenum colorFormat)
		{
			_colorFormat = colorFormat;
		}

	private:
		T* _bytes;
		int _width;
		int _height;
		GLenum _type;
		GLenum _internalFormat;
		GLenum _colorFormat;
		GLenum _textureMode;
		GLuint _samples;
		inline static GLuint _active{0};
		inline static const Texture* _bound[32] = {};
		glm::ivec2 _regionSize{};
		bool _atlas{false};
		inline static unsigned int _bindCount{0};
	};
}

