#pragma once

#include <functional>
#include "glinstancedobject.h"

namespace lithium
{
    class ParticleSystem : public lithium::InstancedObject<glm::mat4>
    {
    public:
        class Particle
        {
        public:
            Particle()
            {
            }

            Particle(const glm::mat4& model, float duration, const glm::vec3& dv, const glm::vec3& ds, const glm::vec3& dr)
                : _model{model}, _duration{duration}, _dv{dv}, _ds{ds}, _dr{dr}, _active{true}
            {
                
            }

            void update(float dt)
            {
                if(_active)
                {
                    _model = glm::translate(_model, _dv * dt);
                    if(_dr.x != 0.0f) _model = glm::rotate(_model, glm::radians(_dr.x * dt), glm::vec3(1.0f, 0.0f, 0.0f));
                    if(_dr.y != 0.0f) _model = glm::rotate(_model, glm::radians(_dr.y * dt), glm::vec3(0.0f, 1.0f, 0.0f));
                    if(_dr.z != 0.0f) _model = glm::rotate(_model, glm::radians(_dr.z * dt), glm::vec3(0.0f, 0.0f, 1.0f));
                    _model = glm::scale(_model, glm::vec3{1.0f} + _ds * dt);
                }
            }

            glm::mat4 model() const
            {
                return _model;
            }

            bool isExpired()
            {
                return _duration <= 0;
            }

            bool active() const
            {
                return _active;
            }

        private:
            bool _active{false};
            glm::mat4 _model;
            float _duration;
            glm::vec3 _dv;
            glm::vec3 _ds;
            glm::vec3 _dr;
        };

        ParticleSystem(lithium::Mesh* mesh, lithium::ImageTexture* diffuse, float spawnInterval, int maxParticles, const std::function<Particle()>& onSpawn)
            : lithium::InstancedObject<glm::mat4>{mesh, diffuse}, _spawnInterval{spawnInterval}, _maxParticles{maxParticles}, _onSpawn{onSpawn}
        {
            
        }

        ParticleSystem(const ParticleSystem& other)
            : lithium::InstancedObject<glm::mat4>{other}, _spawnInterval{other._spawnInterval}, _maxParticles{other._maxParticles}, _onSpawn{other._onSpawn}
        {

        }

        ParticleSystem(const InstancedObject& other) : lithium::InstancedObject<glm::mat4>{other}
        {

        }

        virtual void update(float dt) override
        {
            lithium::Object::update(dt);
            _spawnTime -= dt;
            if(_spawnTime <= 0)
            {
                Particle particle = _onSpawn();
                if(_particles.size() <= _nextParticle)
                {
                    addParticle(particle);
                }
                else
                {
                    _particles[_nextParticle] = particle;
                }
                _spawnTime = _spawnInterval;
                _nextParticle = (_nextParticle + 1) % _maxParticles;
            }
            for(int i{0}; i < _particles.size(); ++i)
            {
                if(_particles[i].active())
                {
                    _particles[i].update(dt);
                    _instances[i] = _particles[i].model();
                }
            }
            allocateBufferData();
        }

        void addParticle(Particle particle)
        {
            _particles.push_back(particle);
            addInstance(particle.model());
        }

        private:
            std::vector<Particle> _particles;
            int _nextParticle{0};
            float _spawnInterval;
            float _spawnTime{0.0f};
            int _maxParticles;
            const std::function<Particle()> _onSpawn;
    };
}