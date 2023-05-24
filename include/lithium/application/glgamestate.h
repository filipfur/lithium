#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <initializer_list>
#include "glupdateable.h"
#include "glscene.h"

namespace lithium
{
    class GameState : lithium::Updateable
    {
    public:
        GameState()
        {

        }

        void setOnEnter(const std::function<void(GameState*)>& onEnter)
        {
            _onEnter = onEnter;
        }

        void setOnExit(const std::function<void(GameState*)>& onExit)
        {
            _onExit = onExit;
        }

        void addScene(std::shared_ptr<lithium::Scene> scene)
        {
            _scenes.push_back(scene);
            scene->copyEntities(_entities);
        }

        void addScenes(const std::initializer_list<std::shared_ptr<Scene>>& scenes)
        {
            for(auto scene : scenes)
            {
                addScene(scene);
            }
        }

        void enter()
        {
            if(_onEnter)
            {
                _onEnter(this);
            }
            for(auto scene : _scenes)
            {
                scene->activate();
            }
        }

        void exit(const GameState& nextState)
        {
            if(_onExit)
            {
                _onExit(this);  
            }
            for(auto scene : _scenes)
            {
                if(!nextState.contains(scene))
                {
                    scene->deactivate();
                }
                else
                {
                    std::cout << "Scene already in next state\n";
                }
            }
        }

        virtual void update(float dt) override
        {
            lithium::Updateable::update(dt);
            for(auto scene : _scenes)
            {
                if(scene->active())
                {
                    scene->update(dt);
                }
            }
        }

        bool forEachScene(std::function<bool(Scene*)> callback)
        {
            int i{0};
            while(callback(_scenes.at(i++).get()) && i < _scenes.size())
            {
                ;
            }
            return i == _scenes.size();
        }

        std::shared_ptr<Scene> scene(size_t index) const
        {
            return _scenes.at(index);
        }

        std::set<ecs::Entity*>& entities()
        {
            return _entities;
        }
        
        bool contains(std::shared_ptr<Scene> scene) const
        {
            return std::find(_scenes.begin(), _scenes.end(), scene) != _scenes.end();
        }

    private:
        std::function<void(GameState*)> _onEnter;
        std::function<void(GameState*)> _onExit;
        std::set<ecs::Entity*> _entities;
        std::vector<std::shared_ptr<Scene>> _scenes;
    };
}