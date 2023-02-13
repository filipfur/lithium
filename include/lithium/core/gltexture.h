#pragma once
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
		Texture(T* buffer, int width, int height, GLenum type=GL_UNSIGNED_BYTE, GLenum internalFormat=GL_RGBA, GLenum colorFormat=GL_RGBA, GLuint unpackAlignment=4, GLenum textureMode=GL_TEXTURE_2D, GLuint samples=4)
			: _bytes{buffer}, _width{width}, _height{height}, _colorFormat{colorFormat}, _textureMode{textureMode}
		{
			glGenTextures(1, &_id);
			setUnpackAlignment(unpackAlignment);
			bind();
			switch(_textureMode)
			{
				case GL_TEXTURE_2D:
					glTexImage2D(_textureMode, 0, internalFormat, _width, _height, 0, colorFormat, type, _bytes);
					break;
				case GL_TEXTURE_2D_MULTISAMPLE:
					glTexImage2DMultisample(_textureMode, samples, internalFormat, _width, _height, GL_TRUE);
					break;
			}
			unbind();
			setFilter()->setWrap();
		}

		virtual ~Texture() noexcept
		{
			glDeleteTextures(1, &_id);
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

		Texture* setFilter(GLenum filter) { return setFilter(filter, filter); }

		Texture* setFilter() { return setFilter(GL_LINEAR, GL_LINEAR); }

		Texture* setWrap(GLenum wrapS, GLenum wrapT)
		{
			bind();
			glTexParameteri(_textureMode, GL_TEXTURE_WRAP_S, wrapS);
			glTexParameteri(_textureMode, GL_TEXTURE_WRAP_T, wrapT);
			unbind();
			return this;
		}

		Texture* setWrap(GLenum wrap) { return setWrap(wrap, wrap); }

		Texture* setWrap() { return setWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE); }

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

		void subdivide(size_t x, size_t y) // not tested
		{
			_regionSize.x = _width / x;
			_regionSize.y = _height / y;
			for(int j=0; j < y; ++j)
			{
				for(int i=0; i < x; ++i)
				{
					glTexSubImage2D(_textureMode, 0, i * _regionSize.x, j * _regionSize.y, _regionSize.x, _regionSize.y, _colorFormat, GL_UNSIGNED_BYTE, _bytes );
				}
			}
			_atlas = true;
		}

		static void activate(GLuint textureUnit)
		{
			if(_active != textureUnit)
			{
				glActiveTexture(textureUnit);
				_active = textureUnit;
			}
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

	protected:
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

	private:
		T* _bytes;
		int _width;
		int _height;
		GLenum _textureMode;
		GLuint _colorFormat;
		inline static GLuint _active{0};
		inline static const Texture* _bound[32] = {};
		/*static_assert(_bound[1] == nullptr);
		static_assert(_bound[4] == nullptr);
		static_assert(_bound[8] == nullptr);
		static_assert(_bound[24] == nullptr);
		static_assert(_bound[31] == nullptr);*/
		glm::ivec2 _regionSize{};
		bool _atlas{false};
		inline static unsigned int _bindCount{0};
	};
}

