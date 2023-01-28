#pragma once

#include "globject.h"
#include "glnode.h"
#include "glskinanimation.h"

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

        virtual lithium::SkinnedObject* clone() const override
        {
            return new lithium::SkinnedObject{*this};
        }

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

        void addSkinAnimation(lithium::SkinAnimation* skinAnimation)
        {
            _animations.emplace(skinAnimation->name(), skinAnimation);
            if(_currentAnimation == nullptr)
            {
                _currentAnimation = skinAnimation;
            }
        }

        lithium::SkinAnimation* nextAnimation()
        {
            bool wasIt{false};
            for(auto entry : _animations)
            {
                if(wasIt)
                {
                    _currentAnimation = entry.second;
                    wasIt = false;
                    break;
                }
                wasIt = entry.second == _currentAnimation;
            }
            if(wasIt)
            {
                _currentAnimation = _animations.begin()->second;
            }

            return _currentAnimation;
        }

        lithium::SkinAnimation* currentAnimation() const
        {
            return _currentAnimation;
        }

        lithium::SkinAnimation* animation(const std::string& key)
        {
            auto it = _animations.find(key);
            if(it != _animations.end())
            {
                return it->second;
            }
            return nullptr;
        }

        lithium::SkinAnimation* setAnimation(const std::string& key)
        {
            _currentAnimation = animation(key);
            return _currentAnimation;
        }

    private:
        lithium::Node* _root{nullptr};
        lithium::Node* _own{nullptr};
        std::vector<lithium::Node*> _joints;
        std::vector<glm::mat4> _inverseBindMatrices;
        std::vector<glm::mat4> _jointMatrices;
        std::map<std::string, lithium::Node*> _nodesByName;
        std::map<std::string, lithium::SkinAnimation*> _animations;
        lithium::SkinAnimation* _currentAnimation{nullptr};
    };
}