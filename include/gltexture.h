#pragma once
#include <string>
#include "glelement.h"
#include <glm/glm.hpp>

namespace lithium
{
	template <typename T=unsigned char, GLenum U=GL_UNSIGNED_BYTE, GLenum V=GL_TEXTURE_2D, GLuint W=4>
	class Texture : public Element
	{
	public:
		Texture(T* buffer, int width, int height, GLenum internalFormat=GL_RGBA, GLenum colorFormat=GL_RGBA, GLuint filter=GL_LINEAR,
			GLuint textureWrap=GL_CLAMP_TO_EDGE, GLuint textureUnit=GL_TEXTURE0, GLuint unpackAlignment=4, bool genMipmaps=false)
			: _bytes{buffer}, _width{width}, _height{height}, _colorFormat{colorFormat}, _textureUnit{textureUnit}
		{
			glGenTextures(1, &_id);
			bind();
			glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment);
			switch(V)
			{
				case GL_TEXTURE_2D:
					glTexImage2D(V, 0, internalFormat, _width, _height, 0, colorFormat, U, _bytes);
					break;
				case GL_TEXTURE_2D_MULTISAMPLE:
					glTexImage2DMultisample(V, W, internalFormat, _width, _height, GL_TRUE);
					break;
			}
			if(genMipmaps)
			{
				glGenerateMipmap(V);
			}
			glTexParameteri(V, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(V, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(V, GL_TEXTURE_WRAP_S, textureWrap);
			glTexParameteri(V, GL_TEXTURE_WRAP_T, textureWrap);

			unbind();
		}

		virtual ~Texture() noexcept
		{
			glDeleteTextures(1, &_id);
		}

		virtual void bind() override
		{
			activate();
			int index = _textureUnit - GL_TEXTURE0;
			if(_bound[index] != this)
			{
				glBindTexture(V, _id);
				_bound[index] = this;
				++_bindCount;
				//std::cout << "binding: " << _name << std::endl;
			}
		}

		void subdivide(size_t x, size_t y) // not tested
		{
			_regionSize.x = _width / x;
			_regionSize.y = _height / y;
			for(int j=0; j < y; ++j)
			{
				for(int i=0; i < x; ++i)
				{
					glTexSubImage2D(V, 0, i * _regionSize.x, j * _regionSize.y, _regionSize.x, _regionSize.y, _colorFormat, GL_UNSIGNED_BYTE, _bytes );
				}
			}
			_atlas = true;
		}

		void activate()
		{
			if(_active != _textureUnit)
			{
				glActiveTexture(_textureUnit);
				glBindTexture(V, _id);
			}
		}

		virtual void unbind() override
		{
			_bound[_textureUnit - GL_TEXTURE0] = nullptr;
			glBindTexture(V, 0);
		}

		static unsigned int countBinds()
		{
			unsigned int binds = _bindCount;
			_bindCount = 0;
			return binds;
		}

	private:
		T* _bytes;
		int _width;
		int _height;
		GLuint _textureUnit;
		GLuint _colorFormat;
		inline static GLuint _active{0};
		inline static const Texture* _bound[3] = {nullptr, nullptr, nullptr};
		glm::ivec2 _regionSize{};
		bool _atlas{false};
		inline static unsigned int _bindCount{0};
	};
}

