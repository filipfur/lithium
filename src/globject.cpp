#include "globject.h"

mygl::Object::Object(mygl::Mesh* mesh, mygl::ShaderProgram* shaderProgram, mygl::Texture* diffuse, mygl::Texture* normal): 
        _mesh{mesh}, _shaderProgram{shaderProgram}, _texture{diffuse}, _normalMap{normal},
        _position{ 0.0f }, _rotation{0.0f, 0.0f, 0.0f, 0.0f}, _scale{1.0f}
{
    updateModel();
    if(shaderProgram == nullptr)
    {
        _visible = false;
    }
}

mygl::Object::Object(const mygl::Object& other) : Object{other._mesh, other._shaderProgram, other._texture, other._specular}
{
    _objectName = other._objectName;
    _iObject = other._iObject;
    _color = other._color;
    _position = other._position;
    _rotation = other._rotation;
    _scale = other._scale;
    _visible = other._visible;
    _normalMap = other._normalMap;
    _textureRegions = other._textureRegions;
    _currentTextureRegion = other._currentTextureRegion;
    _shininess = other._shininess;
    updateModel();
}

mygl::Object::~Object() noexcept
{
    _mesh = nullptr;
    _shaderProgram = nullptr;
    _texture = nullptr;
    _specular = nullptr;
    _objectName = nullptr;
    _iObject = nullptr;
    if(_opacityFader)
    {
        delete _opacityFader;
        _opacityFader = nullptr;
    }
}

void mygl::Object::update(float dt)
{
    if(_flicker > 0)
    {
        _flicker -= dt;
        if(_flicker <= 0)
        {
            _flicker = 0.0f;
            if(_iObject)
            {
                _iObject->onDoneFlickering(this);
            }
        }
    }
    _time += dt;
}

void mygl::Object::draw()
{
    if(_modelInvalidated)
    {
        updateModel();
        _modelInvalidated = false;
    }
    if(!_visible || (((int)(_flicker * 100) % 2) == 1))
    {
        return;
    }

    _shaderProgram->use();

    if (_texture)
    {
        //_shaderProgram->setUniform("u_texture_0", 0);
        _texture->bind();
    }

    if (_specular)
    {
        //_shaderProgram->setUniform("u_specular_0", 1);
        _specular->bind();
    }

    if(_normalMap)
    {
        _shaderProgram->setUniform("u_normal_0", 2);
        _normalMap->bind();
    }
    _mesh->bindVertexArray();

    //_shaderProgram->setUniform("u_time", _time);
    _shaderProgram->setUniform("u_color", fadedColor());
    _shaderProgram->setUniform("u_shininess", _shininess);
    _shaderProgram->setUniform("u_regions", _textureRegions);
    _shaderProgram->setUniform("u_current_region", _currentTextureRegion);
    _shaderProgram->setUniform("u_model", _model);

    //glDepthMask(false);
    this->onDraw();
    //glDepthMask(true);
    
    glActiveTexture(GL_TEXTURE0);
}

void mygl::Object::updateModel()
{
    _model = glm::translate(glm::mat4(1.0f), _position);
    _model = glm::rotate(_model, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _model = glm::rotate(_model, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _model = glm::rotate(_model, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _model = glm::scale(_model, _scale);
}

void mygl::Object::onDraw()
{
    _mesh->drawElements();
}