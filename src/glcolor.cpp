#include "glcolor.h"

#include <sstream>

const lithium::Color lithium::black{0.0f, 0.0f, 0.0f};
const lithium::Color lithium::white(1.0f, 1.0f, 1.0f);

lithium::Color lithium::Color::fromHex(uint32_t hex)
{
    return Color{
        static_cast<float>((hex >> 16) & 0xFF) / 255.0f,
        static_cast<float>((hex >> 8) & 0xFF) / 255.0f,
        static_cast<float>(hex & 0xFF) / 255.0f
    };
}

lithium::Color lithium::Color::fromHex(const std::string& hex)
{
    unsigned int hexValue = 0;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> hexValue;
    return fromHex(hexValue);
}

/*lithium::ColorAlpha lithium::fromHexAlpha(uint32_t hex)
{
    return ColorAlpha{
        static_cast<float>((hex >> 24) & 0xFF) / 255.0f,
        static_cast<float>((hex >> 16) & 0xFF) / 255.0f,
        static_cast<float>((hex >> 8) & 0xFF) / 255.0f,
        static_cast<float>(hex & 0xFF) / 255.0f
    };
}

lithium::ColorAlpha lithium::fromHexAlpha(const std::string& hex)
{
    unsigned int hexValue = 0;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> hexValue;
    return fromHexAlpha(hexValue);
}*/