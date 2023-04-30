#include "globject.h"
#include <glm/gtx/quaternion.hpp>

lithium::Object::Object(std::shared_ptr<lithium::Mesh> mesh, const std::vector<lithium::Object::TexturePointer>& textures)
    : _mesh{mesh}, _textures{textures}
{
    updateModel();
}

lithium::Object::Object(const lithium::Object& other) : Renderable{other}
{
    _mesh = other._mesh;
    _textures = other._textures;
    _objectName = other._objectName;
    _color = other._color;
    _position = other._position;
    _rotation = other._rotation;
    _scale = other._scale;
    updateModel();
}

lithium::Object::~Object() noexcept
{
    _mesh = nullptr;
    _objectName = nullptr;
    _textures.clear();
}

void lithium::Object::shade(lithium::ShaderProgram* shaderProgram)
{
    if(!visible())
    {
        return;
    }
    if(_modelInvalidated)
    {
        updateModel();
    }

    shaderProgram->use();
    shaderProgram->setUniform("u_color", color());
    //shaderProgram->setUniform("u_shininess", _shininess);
    //shaderProgram->setUniform("u_regions", _textureRegions);
    //shaderProgram->setUniform("u_current_region", _currentTextureRegion);
    shaderProgram->setUniform("u_model", _model);
    //shaderProgram->setUniform("u_texture_0", 0);
    //shaderProgram->setUniform("u_specular_0", 1);
    //shaderProgram->setUniform("u_normal_0", 2);
}

void lithium::Object::draw() const
{
    if(!visible())
    {
        return;
    }
    for(int i{0}; i < _textures.size(); ++i)
    {
        _textures[i]->bind(GL_TEXTURE0 + i);
    }
    _mesh->bind();
    _mesh->draw();
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