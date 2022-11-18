#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <functional>

#include "glshaderprogram.h"

namespace lithium
{
	class Camera
	{
	public:
		class Waypoint
		{
		public:
			Waypoint() = delete;

			Waypoint(glm::vec3* position, glm::vec3* offset, float speed, glm::vec3* orientation=nullptr) : _position(position), _offset(offset), _speed(speed), _orientation{orientation}
			{

			}

			Waypoint(const glm::vec3& position, const glm::vec3& offset, float speed) : Waypoint{&_p, &_o, speed}
			{
				_p = position;
				_o = offset;
			}

			Waypoint(const glm::vec3& position, const glm::vec3& offset, float speed, const glm::vec3& orientation) : Waypoint{&_p, &_o, speed, &_v}
			{
				_p = position;
				_o = offset;
				_v = orientation;
			}

			virtual ~Waypoint() noexcept
			{

			}

			const glm::vec3& position() const
			{
				return *_position;
			}
			
			const glm::vec3& offset() const
			{
				return *_offset;
			}

			bool hasOffset() const
			{
				return _offset != nullptr;
			}

			const glm::vec3& orientation() const
			{
				return *_orientation;
			}

			bool hasOrientation() const
			{
				return _orientation != nullptr;
			}

			float speed() const
			{
				return _speed;
			}

			glm::vec3 _p;
			glm::vec3 _o;
			glm::vec3 _v;
			glm::vec3* _position;
			glm::vec3* _orientation;
			glm::vec3* _offset;
			float _speed;
		};

		Camera(const glm::mat4& projection, const glm::vec3& position, const glm::vec3& orientation, const glm::vec3& up);

		glm::mat4 matrix();

		bool followWaypoints(float dt);

		virtual void update(float dt);

		void updateShaders();

		void rotateX(float rotX);
		void rotateY(float rotY);
		void rotateZ(float rotZ);

		void lookDown();

		const glm::vec3& position() const
		{
			return _position;
		}

		void setOffset(const glm::vec3& offset)
		{
			_offset = offset;
		}

		glm::vec3 offset() const { return _offset; }

		void setPosition(const glm::vec3& position)
		{
			_position = position;
		}

		void move(const glm::vec3& delta)
		{
			_position = _position + delta;
		}

		void setOnSequenceFinished(const std::function<void(lithium::Camera*)>& onSequenceFinished)
		{
			_onSequenceFinished = onSequenceFinished;
		}

		glm::vec3 orientation() const
		{
			return _orientation;
		}

		virtual void setOrientation(const glm::vec3& orientation)
		{
			_orientation = orientation;
			_lockedIn = true;
		}

		glm::mat4 rotationMatrix() const;

		void attachShader(lithium::ShaderProgram* shaderProgram)
		{
			_attachedShaders.push_back(shaderProgram);
		}

		void addWaypoint(glm::vec3* position, glm::vec3* offset, float speed)
		{
			_waypoints.push_back(new Waypoint(position, offset, speed));
			_sequenceStarted = true;
		}

		void addWaypoint(glm::vec3 position, glm::vec3 offset, float speed)
		{
			_waypoints.push_back(new Waypoint(position, offset, speed));
			_sequenceStarted = true;
		}

		void addWaypoint(glm::vec3 position, glm::vec3 offset, float speed, glm::vec3 orientation)
		{
			_waypoints.push_back(new Waypoint(position, offset, speed, orientation));
			_sequenceStarted = true;
		}

		bool hasWaypoints() const
		{
			return _currentWaypoint != nullptr || !_waypoints.empty();
		}
		
		void lookAt(const glm::vec3* lookingAt)
		{
			_lookingAt = lookingAt;
			_lockedIn = false;
		}

		void follow(const glm::vec3* following)
		{
			_following = following;
		}

		glm::mat4 view() const
		{
			return _view;
		}

		glm::mat4 projection() const
		{
			return _projection;
		}

		float speed() const
		{
			return _speed;
		}

		void setSpeed(float speed)
		{
			_speed = speed;
		}

		void rotateAroundViewpoint(float degrees)
        {
            auto target = *_lookingAt;
            auto dv =  _position - target;
            float length = glm::length(dv);
            float angle = glm::atan(dv.y, dv.x);
            angle += glm::radians(degrees);
            _position.x = target.x + glm::cos(angle) * length;
			_position.y = target.y + glm::sin(angle) * length;
        }

	protected:
		glm::vec3 _position{0.0f};
		glm::vec3 _orientation;
		glm::vec3 _up;
		glm::vec3 _viewPoint{0.0f};
		glm::vec3 _targetViewPoint;
		glm::mat4 _view;
		glm::mat4 _projection;
		glm::mat4 _matrix;
		glm::mat4 _rotationMatrix;
		bool _sequenceStarted{false};
        bool _lockedIn{false};
		float _speed{3.0f};
		glm::vec3 _offset;
		std::vector<lithium::ShaderProgram*> _attachedShaders;
		Waypoint* _currentWaypoint{nullptr};
		std::vector<Waypoint*> _waypoints;
		const glm::vec3* _lookingAt{nullptr};
		const glm::vec3* _following{nullptr};
		std::function<void(lithium::Camera*)> _onSequenceFinished;
	};
}
