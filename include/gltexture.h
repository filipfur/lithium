#pragma once
#include <string>
#include "glelement.h"
#include <glm/glm.hpp>

namespace mygl
{
	class Texture : public Element
	{
	public:
		Texture(const std::string& name, GLenum internalFormat=GL_RGBA, GLenum colorFormat=GL_RGBA, GLuint filter=GL_LINEAR, GLuint textureWrap=GL_CLAMP_TO_EDGE, GLuint textureUnit=GL_TEXTURE0, GLuint unpackAlignment=4, bool flip=true);
		Texture(unsigned char* buffer, int width, int height, GLenum internalFormat=GL_RGBA, GLenum colorFormat=GL_RGBA, GLuint filter=GL_LINEAR, GLuint textureWrap=GL_CLAMP_TO_EDGE, GLuint textureUnit=GL_TEXTURE0, GLuint unpackAlignment=4);
		~Texture() noexcept;

		void load();

		virtual void bind() override;
		virtual void unbind() override;

		void activate();

		void subdivide(size_t x, size_t y);

		static unsigned int countBinds()
		{
			unsigned int binds = _bindCount;
			_bindCount = 0;
			return binds;
		}

	private:
		unsigned char* _bytes;
		int _width;
		static GLuint _active;
		int _height;
		int _colorChannels;
		GLuint _textureUnit;
		static const Texture* _bound[3];
		GLenum _internalFormat;
		GLenum _colorFormat;
		GLuint _unpackAlignment;
		GLuint _filter;
		GLuint _textureWrap;
		std::string _name;
		glm::ivec2 _regionSize{};
		bool _atlas{false};
		static unsigned int _bindCount;
	};
}

