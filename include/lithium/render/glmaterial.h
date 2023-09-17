#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "glshaderprogram.h"
#include "glimagetexture.h"

namespace lithium
{
    class Material
    {
    public:
        Material(const glm::vec4& baseColor, float metallic=0.0f, float roughness=0.5f) : _baseColor{baseColor}, _metallic{metallic}, _roughness{roughness} {}

        glm::vec4 baseColor() const
        {
            return _baseColor;
        }

        float metallic() const
        {
            return _metallic;
        }

        float roughness() const
        {
            return _roughness;
        }

        void setBaseColor(const glm::vec4& baseColor)
        {
            _baseColor = baseColor;
        }

        void setMetallic(float metallic)
        {
            _metallic = metallic;
        }

        void setRoughness(float roughness)
        {
            _roughness = roughness;
        }

        void setNormalMap(std::shared_ptr<lithium::ImageTexture> normalMap)
        {
            _normalMap = normalMap;
        }

        void setDiffuseMap(std::shared_ptr<lithium::ImageTexture> diffuseMap)
        {
            _diffuseMap = diffuseMap;
        }

        void setArmMap(std::shared_ptr<lithium::ImageTexture> armMap)
        {
            _armMap = armMap;
        }

        std::shared_ptr<lithium::ImageTexture> normalMap() const
        {
            return _normalMap;
        }

        std::shared_ptr<lithium::ImageTexture> diffuseMap() const
        {
            return _diffuseMap;
        }

        std::shared_ptr<lithium::ImageTexture> armMap() const
        {
            return _armMap;
        }

    private:
        glm::vec4 _baseColor{1.0f, 1.0f, 1.0f, 1.0f};
        float _metallic{0.0f};
        float _roughness{0.5f};

        std::shared_ptr<lithium::ImageTexture> _normalMap{nullptr};
        std::shared_ptr<lithium::ImageTexture> _diffuseMap{nullptr};
        std::shared_ptr<lithium::ImageTexture> _armMap{nullptr};
    };
}