#pragma once 

#include "glimagetexture.h"
#include "gljson.h"

namespace lithium
{
    class Font
    {
    public:
        const size_t firstChar{32};

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

            for(int i = firstChar; i < 127; ++i)
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

                _maxCharacterHeight = std::max(_maxCharacterHeight, character.height);

                _characters.push_back(character);
                assert(_characters.size() == i + 1 - firstChar);
            }
        }

        const Character& character(char c)
        {
            assert(c >= firstChar && c < 127);
            return _characters.at(c - firstChar);
        }

        std::shared_ptr<lithium::Texture<unsigned char>> texture() const { return _texture; }
        float width() const { return _width; }
        float height() const { return _height; }
        float size() const { return _size; }

        float maxCharacterHeight() const { return _maxCharacterHeight; }

    private:
        std::shared_ptr<lithium::Texture<unsigned char>> _texture{nullptr};
        lithium::json::Json _fontMetrics;
        
        float _width{0.0f};
        float _height{0.0f};
        float _size{0.0f};
        float _maxCharacterHeight{0.0f};

        std::vector<lithium::Font::Character> _characters;
    };
}