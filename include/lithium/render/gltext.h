#pragma once

#include "globject.h"
#include "glfont.h"

#include <map>

namespace lithium
{
    class Text : public Object
    {
    public:
        Text(std::shared_ptr<Font> font, const std::string& text, float textScale=1.0f);
        Text(const Text& other);
        Text(const Object& other);
        virtual ~Text() noexcept;

        virtual void shade(lithium::ShaderProgram* shaderProgram) override;

        virtual void draw() const override;

        void setText(const std::string& text)
        {
            _text = text;
            initBuffers();
        }

        void setTextScale(float textScale)
        {
            _textScale = textScale;
        }

        float textScale() const
        {
            return _textScale;
        }

        std::string text() const
        {
            return _text;
        }

        glm::vec2 letterXPosition(size_t index)
        {
            return _letterXPositions.at(index);
        }

        virtual Text* clone() const override
        {
            return new Text(*this);
        }

        float width() const
        {
            return _width;
        }

        float height() const
        {
            return _height;
        }

    private:
        void initBuffers();
        std::shared_ptr<Font> _font{nullptr};
        std::string _text{""};
        float _width{0.0f};
        float _height{0.0f};
        std::vector<GLfloat> _vertices;
        std::vector<GLuint> _indices;
        std::vector<glm::vec2> _letterXPositions;
        bool _initialised{false};
        float _textScale;
    };
}