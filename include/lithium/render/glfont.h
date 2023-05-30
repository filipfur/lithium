#pragma once 

#include "glimagetexture.h"
#include "gljson.h"

namespace lithium
{
    class Font
    {
    public:
        struct Character
        {
            float x;
            float y;
            float originX;
            float originY;
            float width;
            float height;
            float advance;
        };

        Font(std::shared_ptr<lithium::Texture<unsigned char>> texture, const std::string& jsonFile) : _texture{texture}
        {
            std::ifstream ifs{jsonFile};
            ifs >> _fontMetrics;
            _width = _fontMetrics["width"].get<float>();
            _height = _fontMetrics["height"].get<float>();
            _size = _fontMetrics["size"].get<float>();

            auto characters = _fontMetrics["characters"];

            for(int i = 32; i < 127; ++i)
            {
                char ch = static_cast<char>(i);
                std::string str = std::string(1, ch);
                auto c = characters[str];

                Character character{
                    c["x"].get<float>(),
                    c["y"].get<float>(),
                    c["originX"].get<float>(),
                    c["originY"].get<float>(),
                    c["width"].get<float>(),
                    c["height"].get<float>(),
                    c["advance"].get<float>()
                };

                _characters.emplace(ch, character);
            }
        }

        const Character& character(char c)
        {
            auto it = _characters.find(c);
            return it->second;
        }

        std::shared_ptr<lithium::Texture<unsigned char>> texture() const { return _texture; }
        float width() const { return _width; }
        float height() const { return _height; }
        float size() const { return _size; }

    private:
        std::shared_ptr<lithium::Texture<unsigned char>> _texture{nullptr};
        lithium::json::Json _fontMetrics;
        
        float _width{0.0f};
        float _height{0.0f};
        float _size{0.0f};

        std::map<char, lithium::Font::Character> _characters;
    };
}