#include "glnode.h"

lithium::Node::Node(const std::string& name, const glm::vec3& position,
            const glm::quat& rotation, const glm::vec3 scale)
            : _name{name}, _position{position}, _rotation{rotation}, _scale{scale}
{
}

lithium::Node::~Node() noexcept
{

}

void lithium::Node::setParent(Node* parent)
{
    _parent = parent;
    parent->addChild(this);
}

void lithium::Node::updateWorldMatrix(const glm::mat4& parentWorldMatrix)
{
    auto it = _children.begin();
    while(it != _children.end())
    {
        Node* node = *it;
        it = _children.erase(it);
        delete node;
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