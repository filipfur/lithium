#pragma once

#include <functional>
#include "glinstancedobject.h"
#include "glparticle.h"

namespace lithium
{
    class ParticleSystem : public lithium::InstancedObject<glm::mat4>
    {
    public:

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