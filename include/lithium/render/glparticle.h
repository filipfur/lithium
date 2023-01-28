#pragma once

#include "gltrs.h"
#include "glupdateable.h"

namespace lithium
{
    class Particle : public TRS, public Updateable
    {
    public:
        Particle()
        {
        }

        Particle(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale, float duration/*const glm::vec3& dv, const glm::vec3& ds, const glm::vec3& dr*/)
            : TRS{position, rotation, scale}, _duration{duration}/*, _dv{dv}, _ds{ds}, _dr{dr}*/, _active{true}
        {
            
        }

        /*void update(float dt)
        {
            if(_active)
            {
                _model = glm::translate(_model, _dv * dt);
                if(_dr.x != 0.0f) _model = glm::rotate(_model, glm::radians(_dr.x * dt), glm::vec3(1.0f, 0.0f, 0.0f));
                if(_dr.y != 0.0f) _model = glm::rotate(_model, glm::radians(_dr.y * dt), glm::vec3(0.0f, 1.0f, 0.0f));
                if(_dr.z != 0.0f) _model = glm::rotate(_model, glm::radians(_dr.z * dt), glm::vec3(0.0f, 0.0f, 1.0f));
                _model = glm::scale(_model, glm::vec3{1.0f} + _ds * dt);
            }
        }*/

        virtual void update(float dt)
        {
            if(_active)
            {
                Updateable::update(dt);
            }
            if(modelInvalidated())
            {
                updateModel();
            }
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
        float _duration;
    };
}