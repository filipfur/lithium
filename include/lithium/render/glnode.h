#pragma once

#include <string>
#include <set>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace lithium
{
    class Node
    {
    public:
        Node(const std::string& name, const glm::vec3& position,
            const glm::quat& rotation, const glm::vec3 scale);
        virtual ~Node() noexcept;

        void setParent(Node* parent);
        void updateWorldMatrix(glm::mat4 parentWorldMatrix, bool doStuff=true);

        std::string name() const
        {
            return _name;
        }

        lithium::Node* parent() const
        {
            return _parent;
        }

        glm::mat4 localMatrix() const
        {
            return _localMatrix;
        }

        glm::mat4 worldMatrix() const
        {
            return _localMatrix;
        }

        glm::vec3 position() const
        {
            return _position;
        }

        glm::quat rotation() const
        {
            return _rotation;
        }

        glm::vec3 scale() const
        {
            return _scale;
        }

        void setPosition(const glm::vec3& position)
        {
            _position = position;
            _modelInvalidated = true;
        }

        void setRotation(const glm::quat& rotation)
        {
            _rotation = rotation;
            _modelInvalidated = true;
        }

        void setScale(const glm::vec3& scale)
        {
            _scale = scale;
            _modelInvalidated = true;
        }

        void forAllChildren(const std::function<void(lithium::Node*)>& callback)
        {
            for(lithium::Node* child : _children)
            {
                callback(child);
            }
        }

        void updateModel()
        {
            _localMatrix = glm::translate(glm::mat4{1.0f}, _position);
            _localMatrix *= glm::toMat4(_rotation);
            _localMatrix = glm::scale(_localMatrix, _scale);
            _modelInvalidated = false;
        }

        void setLocalMatrix(const glm::mat4 matrix)
        {
            _localMatrix = matrix;
            _modelInvalidated = false;
        }

        bool modelInvalidated() const
        {
            return _modelInvalidated;
        }

    private:
        void addChild(Node* child);
        void removeChild(Node* child);

        const std::string _name;
        glm::mat4 _localMatrix{1.0f};
        glm::mat4 _worldMatrix{1.0f};
        glm::vec3 _position{0.0f};
        glm::quat _rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 _scale{1.0f};
        bool _modelInvalidated{true};
        Node* _parent{nullptr};
        std::set<Node*> _children;
    };
}