#pragma once

#include "gltexture.h"
#include "iupdateable.h"
#include "globject.h"
#include "glshaderprogram.h"
#include "nlohmann/json.hpp"

#include <map>

namespace lithium
{
    class Fownt
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

        Fownt(lithium::ShaderProgram* shaderProgram, lithium::Texture* texture, const std::string& jsonFile) : _shaderProgram{shaderProgram}, _texture{texture}
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

        lithium::ShaderProgram* shaderProgram() const { return _shaderProgram; }
        lithium::Texture* texture() const { return _texture; }
        float width() const { return _width; }
        float height() const { return _height; }
        float size() const { return _size; }

    private:
        lithium::ShaderProgram* _shaderProgram{nullptr};
        lithium::Texture* _texture{nullptr};
        nlohmann::json _fontMetrics;
        
        float _width{0.0f};
        float _height{0.0f};
        float _size{0.0f};

        std::map<char, lithium::Fownt::Character> _characters;
    };

    class Text : public Object
    {
    public:
        enum class Alignment{
            LEFT,
            CENTER
        };

        Text(Fownt* font, const std::string& text, float textScale=1.0f);
        Text(const Text& other);
        Text(const Object& other);
        virtual ~Text() noexcept;

        virtual void update(float dt) override;

        virtual void shade(lithium::ShaderProgram* shaderProgram) const override;

        virtual void draw() override;

        void setAlignment(Alignment alignment)
        {
            _alignment = alignment;
        }

        void setText(const std::string& text)
        {
            _text = text;
            initBuffers();
        }

        std::string text() const
        {
            return _text;
        }

        Alignment alignment() const
        {
            return _alignment;
        }

        void setTextScale(float textScale)
        {
            _textScale = textScale;
        }

        virtual void updateModel() override
        {
            glm::vec3 translate{0.0f};
            switch(_alignment)
            {
                case Alignment::CENTER:
                    translate = glm::vec3{-_width / 2.0f, -_height / 2.0f, 0.0f};
                    break;
                default:
                    break;
            }
            _model = glm::translate(glm::mat4(1.0f), _position + translate);
            _model = glm::scale(_model, _scale);
            _model = glm::rotate(_model, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            _model = glm::rotate(_model, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            _model = glm::rotate(_model, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        glm::vec2 letterXPosition(size_t index)
        {
            return _letterXPositions.at(index);
        }

        float width() const {return _width;}

        virtual Text* clone() const override
        {
            return new Text(*this);
        }

    private:
        void initBuffers();
        lithium::Fownt* _font{nullptr};
        std::string _text{""};
        float _width{0.0f};
        float _height{0.0f};
        Alignment _alignment{Alignment::CENTER};
        std::vector<GLfloat> _vertices;
        std::vector<GLuint> _indices;
        std::vector<glm::vec2> _letterXPositions;
        bool _initialised{false};
        float _textScale;
    };
}