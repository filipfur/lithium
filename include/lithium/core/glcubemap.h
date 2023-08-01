#pragma once

#include <memory>
#include <vector>
#include <filesystem>
#include "gltexture.h"

namespace lithium
{
    class Cubemap : public lithium::Texture<unsigned char>
    {
        public:
            using TexturePointer = std::shared_ptr<lithium::Texture<unsigned char>>;
            virtual ~Cubemap() noexcept;

            static std::shared_ptr<Cubemap> load(const std::vector<std::string>& faces);
        private:
            Cubemap();
    };
}