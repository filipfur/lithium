#pragma once

#include <iostream>
#include <stb_image.h>
#include <filesystem>
#include "gltexture.h"
#include "gldebugostream.h"
#include "glfilewatch.h"

namespace lithium
{
    class ImageTexture : public Texture<unsigned char>
    {
        public:
            static ImageTexture* load(const std::filesystem::path& path, GLenum internalFormat=GL_RGBA, GLenum colorFormat=GL_RGBA, GLuint unpackAlignment=4, bool flip=true, bool free=true);

            void save();

            std::filesystem::path path() const;

            ImageTexture* watch();

            virtual void bind() override;

            void reloadBytes();

        private:
            ImageTexture(const std::filesystem::path& path, unsigned char* buffer, int width, int height, GLenum internalFormat, GLenum colorFormat, GLuint unpackAlignment, bool flip);

            void reloadFile();

            const std::filesystem::path _path;
            bool _flip{true};
            std::shared_ptr<FileWatch> _fileWatch;
            bool _changed{false};
    };
}