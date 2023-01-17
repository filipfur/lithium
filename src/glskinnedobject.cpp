#include "glskinnedobject.h"

#include "utility.h"

lithium::SkinnedObject::SkinnedObject(lithium::Mesh* mesh, lithium::ImageTexture* texture, lithium::ImageTexture* specular)
    : lithium::Object{mesh, texture, specular}
{
}

lithium::SkinnedObject::SkinnedObject(const lithium::SkinnedObject& other) : lithium::Object{other}
{
}

lithium::SkinnedObject::~SkinnedObject() noexcept
{

}

void lithium::SkinnedObject::shade(lithium::ShaderProgram* shaderProgram)
{
    lithium::Object::shade(shaderProgram);
    for(lithium::Node* joint : _joints)
    {
        joint->setPosition(_currentAnimation->getPosition(joint->id()));
        joint->setRotation(_currentAnimation->getRotation(joint->id()));
    }
    _own->setLocalMatrix(_model);
    const glm::mat4 identity{1.0f};
    _root->updateWorldMatrix(identity, false);
    updateJointMatrices();
    for(int i=0; i < _jointMatrices.size(); ++i)
    {
        shaderProgram->setUniform("u_jointMatrix[" + std::to_string(i) + "]", _jointMatrices[i]);
    }
}

void lithium::SkinnedObject::update(float dt)
{
    lithium::Object::update(dt);
    _currentAnimation->step(dt);
}

void lithium::SkinnedObject::updateJointMatrices()
{
    const glm::mat4 globalWorldInverse = //glm::inverse(_model); // maybe own.worldMatrix
        glm::inverse(_own->worldMatrix());

    int j{0};
    for(int j{0}; j < _joints.size(); ++j)
    {
        _jointMatrices[j] = globalWorldInverse * _joints[j]->worldMatrix() * _inverseBindMatrices[j];
    }
    
}

void lithium::SkinnedObject::skinData(const std::vector<lithium::Node*>& joints, const std::vector<GLfloat>& inverseBindMatrixData)
{
    _joints = joints;
    for(int i{0}; i < joints.size(); ++i)
    {
        int I = i * 16;
        _inverseBindMatrices.push_back(glm::make_mat4(inverseBindMatrixData.data() + I));
        /*_inverseBindMatrices.push_back(glm::mat4(
            inverseBindMatrixData[I + 0],
            inverseBindMatrixData[I + 1],
            inverseBindMatrixData[I + 2],
            inverseBindMatrixData[I + 3],

            inverseBindMatrixData[I + 4],
            inverseBindMatrixData[I + 5],
            inverseBindMatrixData[I + 6],
            inverseBindMatrixData[I + 7],

            inverseBindMatrixData[I + 8],
            inverseBindMatrixData[I + 9],
            inverseBindMatrixData[I + 10],
            inverseBindMatrixData[I + 11],

            inverseBindMatrixData[I + 12],
            inverseBindMatrixData[I + 13],
            inverseBindMatrixData[I + 14],
            inverseBindMatrixData[I + 15]
        ));*/
        _jointMatrices.push_back(glm::mat4{1.0f});
    }
}