#pragma once 

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <map>
#include <vector>
#include "glassimp_glm_helpers.h"
#include "glanimdata.h"
#include "glanimator.h"
#include "globject.h"
#include "glupdateable.h"
#include "glrenderable.h"

namespace lithium
{
    class Animation;

    class Model : public lithium::Updateable
    {
    public:
        Model()
        {
            _currentAnimation = _animations.end();
        }

        Model(const Model& other) : m_BoneInfoMap{other.m_BoneInfoMap}, _animations{other._animations}
        {
            for(auto object : other._objects)
            {
                _objects.push_back(new Object(*object));
            }
            _currentAnimation = _animations.end();
            playAnimation(0);
        }

        Model* clone() const
        {
            return new Model(*this);
        }

        void playAnimation(size_t index)
        {
            auto it = _animations.begin();
            std::advance(it, index);
            if(it != _animations.end())
            {
                playAnimation(it->first);
            }
        }

        Animation* animation(const std::string& key)
        {
            Animation* anim{nullptr};
            auto it = _animations.find(key);
            if(it != _animations.end())
            {
                anim = it->second;
            }
            return anim;
        }

        void playAnimation(const std::string animationName)
        {
            auto it = _animations.find(animationName);
            if(it != _animations.end() && it != _currentAnimation)
            {
                //std::cout << "changing animation: " << animationName << std::endl;
                _currentAnimation = it;
                _animator.playAnimation(_currentAnimation->second);
            }
        }

        void cycleAnimation()
        {
            if(++_currentAnimation == _animations.end())
            {
                _currentAnimation = _animations.begin();
            }
            _animator.playAnimation(_currentAnimation->second);
            std::cout << "Cycle animation: " << _currentAnimation->first << std::endl;
        }

        virtual void update(float dt) override
        {
            for(auto obj : _objects)
            {
                obj->update(dt);
            }
            _animator.updateAnimation(dt, m_BoneInfoMap);
        }

        void render(lithium::ShaderProgram* shaderProgram)
        {
            shaderProgram->use();
            _animator.animate(shaderProgram);
            for(auto obj : _objects)
            {
                obj->shade(shaderProgram);
                obj->draw();
                //break;
            }
        }

        void syncWithFirst()
        {
            for(int i{1}; i < _objects.size(); ++i)
            {
                _objects[i]->setPosition(_objects[0]->position());
                _objects[i]->setRotation(_objects[0]->rotation());
                _objects[i]->setScale(_objects[0]->scale());
            }
        }

        void forEachObject(const std::function<bool(lithium::Object*)> & callback)
        {
            for(auto obj : _objects)
            {
                if(!callback(obj))
                {
                    break;
                }
            }
        }

        lithium::Object* object()
        {
            return _objects.size() == 0 ? nullptr : _objects.at(0);
        }

        lithium::Object* object(size_t index)
        {
            lithium::Object* object{nullptr};
            if(index < _objects.size())
            {
                object = _objects.at(index);
            }
            return object;
        }

        bool isCurrentAnimation(const std::string& name)
        {
            return _currentAnimation != _animations.end() && _currentAnimation->first == name;
        }

        std::map<std::string,lithium::Animation*>::iterator currentAnimation() const
        {
            return _currentAnimation;
        }

    private:

        std::vector<lithium::Object*> _objects;
        lithium::Animator _animator;
        std::map<std::string,lithium::Animation*> _animations;
        std::map<std::string,lithium::Animation*>::iterator _currentAnimation;
        std::map<std::string, BoneInfo> m_BoneInfoMap;
        friend class ModelLoader;
    };
}