#include "glcamera.h"

lithium::Camera::Camera(const glm::mat4& projection, const glm::vec3& position, const glm::vec3& orientation, const glm::vec3& up)
	: _projection{projection}, _position{ position }, _orientation{orientation}, _up{up}, _offset{0.0f}
{
	
}

bool lithium::Camera::followWaypoints(float dt)
{
	bool rval = false;
	if(_currentWaypoint != nullptr)
	{
		glm::vec3 p{_currentWaypoint->position()};
		glm::vec3 o{_currentWaypoint->hasOffset() ? _currentWaypoint->offset() : _offset};
		glm::vec3 delta = (p + o) - _position;
		glm::normalize(delta);
		
		_position += delta * dt * _currentWaypoint->speed();
		if(glm::distance2((p + o), _position) < 1.0f)
		{
			//_position = _currentWaypoint->position();
			delete _currentWaypoint;
			_currentWaypoint = nullptr;
			if(!_waypoints.empty())
			{
				_currentWaypoint = _waypoints.front();
				_waypoints.erase(_waypoints.begin());
				rval = true;
			}
		}
		else
		{
			rval = true;
		}
	}
	else if(!_waypoints.empty())
	{
		_currentWaypoint = _waypoints.front();
		_waypoints.erase(_waypoints.begin());
		rval = true;
		_lockedIn = false;
	}
	return rval;
}

void lithium::Camera::update(float dt)
{
	if(!followWaypoints(dt))
	{
		if(_following != nullptr)
		{
			glm::vec3 target = (*_following + _offset);
			glm::vec3 delta = (target - _position) * dt * _speed;
			

			if(glm::distance2(target, _position) <= 0.0001)
			{
				_position = target;
			}
			else
			{
				_position += delta;
			}
		}
	}
}

void lithium::Camera::updateShaders()
{
	for(auto shader : _attachedShaders)
	{
		shader->use();
		shader->setUniform("u_camera", _matrix);
		shader->setUniform("u_camera_position", _position);
	}
}

glm::mat4 lithium::Camera::matrix()
{
	return _matrix;
}

glm::mat4 lithium::Camera::rotationMatrix() const
{
	return _rotationMatrix;
}

void lithium::Camera::rotateX(float rotX)
{
	_orientation = glm::rotate(_orientation, glm::radians(rotX), glm::normalize(glm::cross(_orientation, _up)));
}

void lithium::Camera::rotateY(float rotY)
{
	_orientation = glm::rotate(_orientation, glm::radians(rotY), _up);
}

void lithium::Camera::rotateZ(float rotZ)
{
	_orientation = glm::rotate(_orientation, glm::radians(rotZ), glm::vec3{0.0f, 0.0f, 1.0f}); // ????
}

void lithium::Camera::lookDown()
{
	rotateX(-90.0f);
}