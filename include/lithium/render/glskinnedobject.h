#pragma once

#include "globject.h"
#include "glnode.h"

namespace lithium
{
    class SkinnedObject : public lithium::Object
    {
    public:
        SkinnedObject(lithium::Mesh* mesh, lithium::ImageTexture* texture, lithium::ImageTexture* specular=nullptr);
        SkinnedObject(const SkinnedObject& other);
        virtual ~SkinnedObject() noexcept;

        virtual void update(float dt) override;

        virtual void shade(lithium::ShaderProgram* shaderProgram) override;

        void updateJointMatrices();

        void skinData(const std::vector<lithium::Node*>& joints, const std::vector<GLfloat>& inverseBindMatrixData);

        void setRoot(lithium::Node* root)
        {
            _root = root;
        }

        void setOwn(lithium::Node* own)
        {
            _own = own;
        }

        void addNode(Node* node)
        {
            _nodesByName.emplace(node->name(), node);
        }

        Node* getNode(const std::string& name)
        {
            return _nodesByName[name];
        }

    private:
        lithium::Node* _root{nullptr};
        lithium::Node* _own{nullptr};
        std::vector<lithium::Node*> _joints;
        std::vector<glm::mat4> _inverseBindMatrices;
        std::vector<glm::mat4> _jointMatrices;
        std::map<std::string, lithium::Node*> _nodesByName;
    };
}