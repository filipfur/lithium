#pragma once

#include <iostream>
#include <stb_image.h>
#include <filesystem>
#include "gltexture.h"
#include "gldebugostream.h"

namespace lithium
{
    class ImageTexture : public Texture<unsigned char>
    {
        public:
            static ImageTexture* load(const std::filesystem::path& path, GLenum internalFormat=GL_RGBA, GLenum colorFormat=GL_RGBA, GLuint unpackAlignment=4, bool flip=true)
            {
                stbi_set_flip_vertically_on_load(flip);
                int width, height, colorChannels;
                unsigned char* bytes = stbi_load(path.string().c_str(), &width, &height, &colorChannels, 0);
                if(colorChannels == 3 && colorFormat == GL_RGBA)
                {
                    colorFormat = GL_RGB;
                }
                else if(colorChannels == 4 && colorFormat == GL_RGB)
                {
                    colorFormat = GL_RGBA;
                }
                if (bytes == nullptr)
                {
                    std::cerr << stbi_failure_reason() << std::endl;
                }
                ImageTexture* imageTexture = new lithium::ImageTexture(path, bytes, static_cast<GLuint>(width), static_cast<GLuint>(height),
                    internalFormat, colorFormat, unpackAlignment);
                stbi_image_free(bytes);
                return imageTexture;
            }

            std::filesystem::path path() const
            {
                return _path;
            }

        private:
            ImageTexture(const std::filesystem::path& path, unsigned char* buffer, int width, int height, GLenum internalFormat, GLenum colorFormat, GLuint unpackAlignment=4)
            : Texture<unsigned char>{buffer, width, height, GL_UNSIGNED_BYTE, internalFormat, colorFormat, unpackAlignment, GL_TEXTURE_2D}, _path{path}
            {

            }
        
            const std::filesystem::path _path;
    };
}