#pragma once

#include <filesystem>
#include "gltexture.h"
#include "glsimplecamera.h"

namespace lithium
{
    class CubemapHDR : public lithium::Texture<float>
    {
        public:
            virtual ~CubemapHDR() noexcept;

            static std::shared_ptr<CubemapHDR> load(const std::filesystem::path& path);

            std::shared_ptr<Texture<float>> irradianceMap() const
            {
                return _irradianceMap;
            }

            std::shared_ptr<Texture<float>> prefilterMap() const
            {
                return _prefilterMap;
            }

            std::shared_ptr<Texture<float>> brdfLUT() const
            {
                return _brdfLUT;
            }

            static void draw(CubemapHDR& cubemapHDR, const lithium::SimpleCamera& camera);

        private:
            CubemapHDR();
            std::shared_ptr<Texture<float>> _irradianceMap;
            std::shared_ptr<Texture<float>> _prefilterMap;
            std::shared_ptr<Texture<float>> _brdfLUT;
    };
}