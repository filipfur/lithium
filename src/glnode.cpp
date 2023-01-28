#include "glnode.h"

lithium::Node::Node(const int id, const std::string& name, const glm::vec3& position,
            const glm::quat& rotation, const glm::vec3 scale)
            : TRS{position, rotation, scale}, _id{id}, _name{name}
{
}

lithium::Node::~Node() noexcept
{
    auto it = _children.begin();
    while(it != _children.end())
    {
        Node* node = *it;
        it = _children.erase(it);
        delete node;
    }
}

void lithium::Node::setParent(Node* parent)
{
    _parent = parent;
    parent->addChild(this);
}

void lithium::Node::updateWorldMatrix(glm::mat4 parentWorldMatrix, bool doStuff)
{
    if(this->modelInvalidated())
    {
        this->updateModel();
    }
    if(doStuff)
    {
        _worldMatrix = parentWorldMatrix * _model;
    }
    else
    {
        _worldMatrix = _model;
    }

    for(auto child : _children)
    {
        child->updateWorldMatrix(_worldMatrix);
    }
}

void lithium::Node::addChild(Node* child)
{
    _children.emplace(child);
}

void lithium::Node::removeChild(Node* child)
{
    auto it = _children.find(child);
    _children.erase(it);
    delete *it;
}