#pragma once

#include "glinstancedobject.h"

namespace lithium
{
    class ParticleSystem : public lithium::InstancedObject<glm::mat4>
    {
    public:
        class Particle
        {
        public:
            Particle(const glm::mat4& model, float duration, const glm::vec3& dv, const glm::vec3& ds, const glm::vec3& dr)
                : _model{model}, _duration{duration}, _dv{dv}, _ds{ds}, _dr{dr}
            {
                
            }

            void update(float dt)
            {
                _model = glm::translate(_model, _dv * dt);
                //_model = glm::scale(_model, glm::vec3{1.0f} + _ds * dt);
            }

            glm::mat4 model() const
            {
                return _model;
            }

            bool isExpired()
            {
                return _duration <= 0;
            }

        private:
            glm::mat4 _model;
            float _duration;
            glm::vec3 _dv;
            glm::vec3 _ds;
            glm::vec3 _dr;
        };

        ParticleSystem(lithium::Mesh* mesh, lithium::ImageTexture* diffuse=nullptr, lithium::ImageTexture* normal=nullptr)
            : lithium::InstancedObject<glm::mat4>{mesh, diffuse, normal}
        {
            
        }

        ParticleSystem(const ParticleSystem& other)
            : lithium::InstancedObject<glm::mat4>{other}
        {

        }

        ParticleSystem(const InstancedObject& other) : lithium::InstancedObject<glm::mat4>{other}
        {

        }

        virtual void update(float dt) override
        {
            lithium::Object::update(dt);
            for(int i{0}; i < _particles.size(); ++i)
            {
                Particle* particle = _particles[i];
                particle->update(dt);
                _instances[i] = particle->model();
            }
            allocateBufferData();
        }

        void addParticle(Particle* particle)
        {
            _particles.push_back(particle);
            addInstance(particle->model());
        }

        private:
            std::vector<Particle*> _particles;
    };
}