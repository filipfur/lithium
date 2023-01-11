#pragma once

#include <string>
#include <glm/glm.hpp>

namespace lithium
{
    class Node
    {
    public:
        Node(const std::string& name, const glm::vec3& position,
            const glm::quat& rotation, const glm::vec3 scale);
        virtual ~Node() noexcept;

        void setParent(Node* parent);
        void updateWorldMatrix(const glm::mat4& parentWorldMatrix);

    private:
        void addChild(Node* child);
        void removeChild(Node* child);

        const std::string _name;
        glm::mat4 _localMatrix{1.0f};
        glm::mat4 _worldMatrix{1.0f};
        glm::vec3 _position{0.0f};
        glm::quat _rotation{0.0f};
        glm::vec3 _scale{0.0f};
        Node* _parent{nullptr};
        std::set<Node*> _children;
    };
}