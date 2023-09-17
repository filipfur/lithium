#include "glimagetexture.h"

lithium::ImageTexture* lithium::ImageTexture::load(const std::filesystem::path& path, GLenum internalFormat, GLenum colorFormat, GLuint unpackAlignment, bool flip)
{
    stbi_set_flip_vertically_on_load(flip);
    int width, height, colorChannels;
    unsigned char* bytes = stbi_load(path.string().c_str(), &width, &height, &colorChannels, 0);
    if (bytes == nullptr)
    {
        std::cerr << stbi_failure_reason() << std::endl;
    }
    if(colorChannels == 3 && colorFormat == GL_RGBA)
    {
        colorFormat = GL_RGB;
    }
    else if(colorChannels == 4 && colorFormat == GL_RGB)
    {
        colorFormat = GL_RGBA;
    }
    else if(colorChannels == 1)
    {
        colorFormat = GL_RED;
        internalFormat = GL_RED;
    }
    ImageTexture* imageTexture = new lithium::ImageTexture(path, bytes, static_cast<GLuint>(width), static_cast<GLuint>(height),
        internalFormat, colorFormat, unpackAlignment, flip);
    stbi_image_free(bytes);
    return imageTexture;
}

std::filesystem::path lithium::ImageTexture::path() const
{
    return _path;
}

lithium::ImageTexture::ImageTexture(const std::filesystem::path& path, unsigned char* buffer, int width, int height, GLenum internalFormat, GLenum colorFormat, GLuint unpackAlignment, bool flip)
: Texture<unsigned char>{buffer, width, height, GL_UNSIGNED_BYTE, internalFormat, colorFormat, GL_TEXTURE_2D}, _path{path}, _flip{flip}
{

}

void lithium::ImageTexture::bind()
{
    lithium::Texture<unsigned char>::bind();
    if(_changed)
    {
        reload();
    }
}

void lithium::ImageTexture::reload()
{
    stbi_set_flip_vertically_on_load(_flip);
    int width, height, colorChannels;
    unsigned char* bytes = stbi_load(_path.string().c_str(), &width, &height, &colorChannels, 0);
    if (bytes == nullptr)
    {
        std::cerr << stbi_failure_reason() << std::endl;
    }
    if(colorChannels == 3 && colorFormat() == GL_RGBA)
    {
        setColorFormat(GL_RGB);
    }
    else if(colorChannels == 4 && colorFormat() == GL_RGB)
    {
        setColorFormat(GL_RGBA);
    }
    /*glDeleteTextures(1, &_id);
    glGenTextures(1, &_id);
    setWidth(static_cast<GLuint>(width));
    setHeight(static_cast<GLuint>(height));
    bind();*/
    lithium::Texture<unsigned char>::loadBytes(bytes);
    errorCheck();
    stbi_image_free(bytes);
    //unbind();
    _changed = false;
}

lithium::ImageTexture* lithium::ImageTexture::watch()
{
    _fileWatch = FileWatch::start(_path, [this](const std::filesystem::path& path)
    {
        std::cout << "Image file changed: " << path << std::endl;   
        _changed = true;
    });
    return this;
}