#pragma once

#include "glupdateable.h"
#include "globject.h"
#include "glfont.h"

#include <map>

namespace lithium
{
    class Text : public Object
    {
    public:
        enum class Alignment{
            LEFT,
            CENTER
        };

        Text(Font* font, const std::string& text, float textScale=1.0f);
        Text(const Text& other);
        Text(const Object& other);
        virtual ~Text() noexcept;

        virtual void update(float dt) override;

        virtual void shade(lithium::ShaderProgram* shaderProgram) override;

        virtual void draw() const override;

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
        lithium::Font* _font{nullptr};
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