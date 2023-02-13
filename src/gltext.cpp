#include "gltext.h"

lithium::Text::Text(std::shared_ptr<Font> font, const std::string& text, float textScale)
    : lithium::Object{nullptr, {font->texture()}}, _font{font}, _textScale{textScale}
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

void lithium::Text::update(float dt)
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
            _mesh->vertexArrayBuffer(0)->allocate(_vertices);
            //_mesh->elementArrayBuffer()->bind();
            _mesh->elementArrayBuffer()->allocate(_indices);
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

void lithium::Text::initBuffers()
{
    float x = 0.0f;
    float y = 0.0f;//fontsize / 2.0f;

    _vertices.clear();
    _indices.clear();
    _letterXPositions.clear();

    _height = 0;

    for(int i = 0; i < _text.length(); ++i)
    {
        //auto c = font->characters[_text[i]];
        auto c = _font->character(_text[i]);
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

        GLuint curidx = i * 4;
        _indices.insert(_indices.end(), {
            curidx+0, curidx+2, curidx+1,  // first triangle
            curidx+1, curidx+2, curidx+3}); // second triangle
        float oldX = x;
        x += c.advance * _textScale;
        _letterXPositions.push_back(glm::vec2{oldX, x});
        _height = _height < c.height * _textScale ? c.height * _textScale : _height;
    }
    _width = x;
    _initialised = false;
    setModelInvalidated(true);
}