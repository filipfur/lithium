#include "gltexture.h"

#include <iostream>
#include <stb_image.h>
#include "gldebugostream.h"

lithium::Texture::Texture(const std::string& name, GLenum internalFormat, GLenum colorFormat, GLuint filter, GLuint textureWrap, GLuint textureUnit, GLuint unpackAlignment, bool flip) : _bytes { nullptr }, _name{name},
	_internalFormat{internalFormat}, _colorFormat{colorFormat}, _filter{filter}, _textureWrap{textureWrap}, _textureUnit{textureUnit}, _unpackAlignment{unpackAlignment}
{
	stbi_set_flip_vertically_on_load(flip);
	_bytes = stbi_load(name.c_str(), &_width, &_height, &_colorChannels, 0);
	if (_bytes == nullptr)
	{
		std::cerr << stbi_failure_reason() << std::endl;
	}
	load();
	stbi_image_free(_bytes);
}

lithium::Texture::Texture(unsigned char* buffer, int width, int height, GLenum internalFormat,
	GLenum colorFormat, GLuint filter, GLuint textureWrap, GLuint textureUnit,  GLuint unpackAlignment) : _bytes{buffer}, _width{width}, _height{height},
	_internalFormat{internalFormat}, _colorFormat{colorFormat}, _filter{filter}, _textureWrap{textureWrap}, _textureUnit{textureUnit}, _unpackAlignment{unpackAlignment}
{
	load();
}

void lithium::Texture::load()
{
	glGenTextures(1, &_id);
	//glActiveTexture(slot);
	//activate();
	bind();
	/*float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);*/
	glPixelStorei(GL_UNPACK_ALIGNMENT, _unpackAlignment);
	glTexImage2D(GL_TEXTURE_2D, 0, _internalFormat, _width, _height, 0, _colorFormat, GL_UNSIGNED_BYTE, _bytes);
	if(_unpackAlignment == 4)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _textureWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _textureWrap);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, unpackAlignment);

	unbind();
}

lithium::Texture::~Texture() noexcept
{
	glDeleteTextures(1, &_id);
}

GLuint lithium::Texture::_active{0};
const lithium::Texture* lithium::Texture::_bound[3] = {nullptr, nullptr, nullptr};

void lithium::Texture::bind()
{
	activate();
	int index = _textureUnit - GL_TEXTURE0;
	if(_bound[index] != this)
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		_bound[index] = this;
		++_bindCount;
		//std::cout << "binding: " << _name << std::endl;
	}
}

unsigned int lithium::Texture::_bindCount{0};

void lithium::Texture::subdivide(size_t x, size_t y) // not tested
{
	_regionSize.x = _width / x;
	_regionSize.y = _height / y;
	for(int j=0; j < y; ++j)
	{
		for(int i=0; i < x; ++i)
		{
			glTexSubImage2D(GL_TEXTURE_2D, 0, i * _regionSize.x, j * _regionSize.y, _regionSize.x, _regionSize.y, _colorFormat, GL_UNSIGNED_BYTE, _bytes );
		}
	}
	_atlas = true;
}

void lithium::Texture::activate()
{
	if(_active != _textureUnit)
	{
		glActiveTexture(_textureUnit);
		glBindTexture(GL_TEXTURE_2D, _id);
	}
}

void lithium::Texture::unbind()
{
	_bound[_textureUnit - GL_TEXTURE0] = nullptr;
	glBindTexture(GL_TEXTURE_2D, 0);
}