#pragma once

#include "globject.h"
#include "glfont.h"

#include <map>

namespace lithium
{
    class Text : public Object
    {
    public:
        enum Alignment {
            LEFT,
            CENTER
        };

        Text(std::shared_ptr<Font> font, const std::string& text, float textScale=1.0f, Alignment alignment=Alignment::LEFT, float lineSpacing=1.0f, float letterSpacing=1.0f);
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

        float maxLineWidth() const
        {
            return _maxLineWidth;
        }

        void setMaxLineWidth(float maxLineWidth)
        {
            _maxLineWidth = maxLineWidth;
            initBuffers();
        }

        void setAlignment(Alignment alignment)
        {
            _alignment = alignment;
            initBuffers();
        }

        void setLineSpacing(float lineSpacing)
        {
            _lineSpacing = lineSpacing;
            initBuffers();
        }

        void setLetterSpacing(float letterSpacing)
        {
            _letterSpacing = letterSpacing;
            initBuffers();
        }

    private:
        void measureText();
        void initBuffers();

        std::shared_ptr<Font> _font{nullptr};
        std::string _text{""};
        float _width{0.0f};
        float _height{0.0f};
        float _lineSpacing{1.0f};
        float _letterSpacing{1.0f};
        float _yOffset{0.0f};
        std::vector<GLfloat> _vertices;
        std::vector<GLuint> _indices;
        std::vector<glm::vec2> _letterXPositions;
        bool _initialised{false};
        float _textScale;
        float _maxLineWidth{0.0f};
        std::vector<std::string> _lines;
        std::vector<float> _lineWidths;
        Alignment _alignment{Alignment::LEFT};
    };
}