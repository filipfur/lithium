#include "gltext.h"

lithium::Text::Text(std::shared_ptr<Font> font, const std::string& text, float textScale, Alignment alignment, float lineSpacing, float letterSpacing)
    : lithium::Object{nullptr, {font->texture()}}, _font{font}, _textScale{textScale}, _alignment{alignment}, _lineSpacing{lineSpacing}, _letterSpacing{letterSpacing}
{
    setText(text);
}

lithium::Text::Text(const lithium::Text& other) : lithium::Object{other}, _font{other._font}, _textScale{other._textScale}, _width{other._width}, _height{other._height}
{
    setText(other._text);
}

lithium::Text::Text(const lithium::Object& other) : lithium::Object{other}
{

}

lithium::Text::~Text() noexcept
{

}

void lithium::Text::shade(lithium::ShaderProgram* shaderProgram)
{
    if(!visible())
    {
        return;
    }
    if(modelInvalidated())
    {
        updateModel();
    }

    if(!_initialised)
    {
        _initialised = true;
        if(_mesh != nullptr)
        {
            _mesh->bind();
            //_mesh->vertexArrayBuffer(0)->bind();
            _mesh->vertexArray()->vertexArrayBuffer(0)->allocate(_vertices);
            //_mesh->elementArrayBuffer()->bind();
            _mesh->vertexArray()->elementArrayBuffer()->allocate(_indices);
            //delete _mesh;
            //_mesh = nullptr;
        }
        else
        {
            static const std::vector<lithium::VertexArrayBuffer::AttributeType> attribs = {lithium::VertexArrayBuffer::AttributeType::VEC4};
            _mesh = std::make_shared<lithium::Mesh>(attribs, _vertices, _indices);
            setMesh(_mesh);
        }
    }
    shaderProgram->use();
    //TODO: Optimize, remove all shader dynamic shader and fix everything in initBuffer thread (vertex attributes).
    shaderProgram->setUniform("u_texture", 0);
    shaderProgram->setUniform("u_color", color());
    shaderProgram->setUniform("u_model", _model);
}

void lithium::Text::draw() const
{
    if(!visible())
    {
        return;
    }

    _mesh->bind();
    _font->texture()->bind(GL_TEXTURE0);

    glDisable(GL_DEPTH_TEST); // TODO: This is a hack due to letter quads overlapping.
    _mesh->draw();
    glEnable(GL_DEPTH_TEST);
    _mesh->unbind();
}

void lithium::Text::measureText()
{
    _width = 0;
    _height = 0;
    _lines.clear();
    _lineWidths.clear();

    float x = 0;
    std::string line{""};
    int numCharacters = 0;
    float line0OriginY{0};
    float y = 0.0f;
    float lineNY2{0.0f};
    for(int i{0}; i < _text.length(); ++i)
    {
        if(_text[i] == '\n')
        {
            _width = std::max(_width, x);
            _lineWidths.push_back(x);
            _lines.push_back(line);
            line = "";
            x = 0;
            y += _font->maxCharacterHeight() * _lineSpacing * _textScale;
            continue;
        }
        auto c = _font->character(_text[i]);
        if(_lines.size() == 0)
        {
            line0OriginY = std::max(c.originY, line0OriginY);
        }
        ++numCharacters;
        line += _text[i];
        x += c.advance * _textScale * _letterSpacing;
        float y2 = y - c.originY * _textScale + c.height * _textScale;
        lineNY2 = std::max(lineNY2, y2);
    }

    if(x > 0)
    {
        _width = std::max(_width, x);
        _lineWidths.push_back(x);
        _lines.push_back(line);
    }

    _yOffset = -line0OriginY * _textScale;
    _height = lineNY2 - _yOffset;
}

void lithium::Text::initBuffers()
{
    float x = 0.0f;
    float y = 0.0f;//fontsize / 2.0f;

    _vertices.clear();
    _indices.clear();
    _letterXPositions.clear();

    measureText();

    GLuint curidx = 0;//i * 4;

    switch(_alignment)
    {
        case Alignment::CENTER:
            y = -_yOffset - _height * 0.5f;
            break;
    }

    for(int lineNo = 0; lineNo < _lines.size(); ++lineNo)
    {
        const auto& line = _lines[lineNo];

        switch(_alignment)
        {
            case Alignment::LEFT:
                x = 0.0f;
                break;
            case Alignment::CENTER:
                x = (_width - _lineWidths[lineNo]) / 2.0f - _width * 0.5f;
                break;
        }

        for(int i = 0; i < line.length(); ++i)
        {
            auto c = _font->character(line[i]);
            float x0 = x - c.originX * _textScale;
            float y0 = y - c.originY * _textScale;
            float s0 = c.x / _font->width();
            float t0 = c.y / _font->height();

            float w = c.width * _textScale;

            float x1 = x - c.originX * _textScale + w;
            float s1 = (c.x + c.width) / _font->width();

            float y2 = y - c.originY * _textScale + c.height * _textScale;
            float t2 = (c.y + c.height) / _font->height();


            _vertices.insert(_vertices.end(), { 
                x0, y0, s0, t0,
                x1, y0, s1, t0,  
                x0, y2, s0, t2,  
                x1, y2, s1, t2});

            // p0 --- p1
            // | \     |
            // |   \   |
            // |     \ |
            // p2 --- p3

            _indices.insert(_indices.end(), {
                curidx+0, curidx+2, curidx+1,  // first triangle
                curidx+1, curidx+2, curidx+3}); // second triangle
            curidx += 4;
            float oldX = x;
            x += c.advance * _textScale * _letterSpacing;
            _letterXPositions.push_back(glm::vec2{oldX, x});
        }

        y += _font->maxCharacterHeight() * _lineSpacing * _textScale;
    }


    _initialised = false;
    setModelInvalidated(true);
}