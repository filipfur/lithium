#include "globject.h"
#include <glm/gtx/quaternion.hpp>

lithium::Object::Object(lithium::Mesh* mesh, lithium::ImageTexture* diffuse, lithium::ImageTexture* normal): 
        _mesh{mesh}, _texture{diffuse}, _normalMap{normal},
        _position{ 0.0f }, _rotation{0.0f, 0.0f, 0.0f}, _scale{1.0f}
{
    updateModel();
}

lithium::Object::Object(const lithium::Object& other) : Object{other._mesh, other._texture, other._specular}
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

lithium::Object::~Object() noexcept
{
    _mesh = nullptr;
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

void lithium::Object::update(float dt)
{
    lithium::Updateable::update(dt);

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
}

void lithium::Object::shade(lithium::ShaderProgram* shaderProgram)
{
    if(!_visible)
    {
        return;
    }
    if(_modelInvalidated)
    {
        updateModel();
    }

    shaderProgram->use();
    shaderProgram->setUniform("u_color", fadedColor());
    //shaderProgram->setUniform("u_shininess", _shininess);
    //shaderProgram->setUniform("u_regions", _textureRegions);
    //shaderProgram->setUniform("u_current_region", _currentTextureRegion);
    shaderProgram->setUniform("u_model", _model);
    //shaderProgram->setUniform("u_texture_0", 0);
    //shaderProgram->setUniform("u_specular_0", 1);
    //shaderProgram->setUniform("u_normal_0", 2);
}

void lithium::Object::draw()
{
    if(!_visible || (((int)(_flicker * 100) % 2) == 1))
    {
        return;
    }

    if (_texture)
    {
        _texture->bind();
    }

    if (_specular)
    {
        _specular->bind();
    }

    if(_normalMap)
    {
        _normalMap->bind();

    }

    _mesh->bind();

    //glDepthMask(false);
    _mesh->draw();
    //glDepthMask(true);
    
    glActiveTexture(GL_TEXTURE0);
}

void lithium::Object::updateModel()
{
    _model = glm::translate(glm::mat4(1.0f), _position);
    _model = glm::rotate(_model, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _model = glm::rotate(_model, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    _model = glm::rotate(_model, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    _model = glm::scale(_model, _scale);
    _modelInvalidated = false;
}