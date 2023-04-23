#pragma once

#include "globject.h"
#include "glshaderprogram.h"

namespace lithium
{
    class Light : public Object
    {
    public:
        Light(std::shared_ptr<lithium::Mesh> mesh=nullptr, const std::vector<Object::TexturePointer>& textures={});
        Light(const Light& other);
        Light(const Object& other);
        virtual ~Light() noexcept;

        void setLightColor(const glm::vec3& lightColor)
        {
            _lightColor = lightColor;
        }

        void setIntensity(float intensity)
        {
            _intensity = intensity;
        }

        const glm::vec3& lightColor() const
        {
            return _lightColor;
        }

        float intensity() const
        {
            return _intensity;
        }

    protected:
        glm::vec3 _lightColor{1.0f, 1.0f, 1.0f};
        float _intensity{1.0f};
    };
}