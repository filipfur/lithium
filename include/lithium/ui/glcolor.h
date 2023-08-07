#pragma once

#include <glm/glm.hpp>
#include <string>

namespace lithium
{
    class Color
    {
    public:
        Color(float r, float g, float b) : _color{r, g, b} {}

        Color(const glm::vec3& color) : _color(color) {}

        Color(float c) : _color(c, c, c) {}

        Color(uint32_t hex) : _color(fromHex(hex)) {}
        Color(int hex) : _color(fromHex(static_cast<uint32_t>(hex))) {}

        Color(const std::string& hex) : _color(fromHex(hex)) {}

        operator glm::vec3() const
        {
            return _color;
        }

        operator uint32_t() const
        {
            return static_cast<uint32_t>(_color.x) << 16 | static_cast<uint32_t>(_color.y) << 8 | static_cast<uint32_t>(_color.z);
        }

        static Color fromHex(uint32_t hex);

        static Color fromHex(const std::string& hex);

    private:
        glm::vec3 _color;
    };

    extern const Color black;
    extern const Color white;
}