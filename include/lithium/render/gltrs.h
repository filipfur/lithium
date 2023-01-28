#pragma once

#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

namespace lithium
{
    class TRS
    {
    public:
        TRS()
        {

        }

        TRS(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
            : _position{position}, _rotation{rotation}, _scale{scale}
        {

        }

        virtual ~TRS() noexcept
        {

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

        void updateModel()
        {
            _model = glm::translate(glm::mat4{1.0f}, _position);
            _model *= glm::toMat4(_rotation);
            _model = glm::scale(_model, _scale);
            _modelInvalidated = false;
        }

        bool modelInvalidated() const
        {
            return _modelInvalidated;
        }

        glm::mat4 model() const
        {
            return _model;
        }

        void translate(const glm::vec3& translation)
        {
            _model = glm::translate(_model, translation);
        }

        void scale(const glm::vec3& scale)
        {
            _model = glm::scale(_model, scale);
        }

    protected:
        glm::vec3 _position{0.0f};
        glm::quat _rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 _scale{1.0f};
        glm::mat4 _model{1.0f};
        bool _modelInvalidated{true};
    };
}