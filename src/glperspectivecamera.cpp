#include "glperspectivecamera.h"


mygl::PerspectiveCamera::~PerspectiveCamera() noexcept
{

}


/*void mygl::PerspectiveCamera::update(float dt)
{
	mygl::Camera::update(dt);
	glm::mat4 view;

	if(_currentWaypoint != nullptr)
	{
		if(_currentWaypoint->hasOrientation())
		{
			view = glm::lookAt(_position, _position + _currentWaypoint->orientation(), _up);
		}
		else
		{
			glm::vec3 p{_currentWaypoint->position()};
			view = glm::lookAt(_position, p, _up);
		}
	}	
	else if(_lookingAt != nullptr)
	{
		view = glm::lookAt(_position, *_lookingAt, _up);
	}
	else
	{
		view = glm::lookAt(_position, _position + _orientation, _up);
	}
	_matrix = _projection * view;
	_rotationMatrix = _projection * glm::mat4(glm::mat3(view));
	updateShaders();
}*/


void mygl::PerspectiveCamera::update(float dt)
{
	mygl::Camera::update(dt);
	glm::mat4 view;
	bool looking = false;

	if(_lockedIn)
	{
		view = glm::lookAt(_position, _position + _orientation, _up);
	}
	else
	{
		if(_currentWaypoint != nullptr)
		{
			if(_currentWaypoint->hasOrientation())
			{
				_targetEyePoint = _position + _currentWaypoint->orientation();
				_lockedIn = true;
				_orientation = _currentWaypoint->orientation();
			}
			else
			{
				_targetEyePoint = _currentWaypoint->position();
			}
		}
		else if(_lookingAt != nullptr)
		{
			_targetEyePoint = *_lookingAt;
			looking = true;
		}
		else
		{
			_targetEyePoint = _position + _orientation;
		}
		if(glm::distance2(_eyePoint, _targetEyePoint) < 0.0001)
		{
			_eyePoint = _targetEyePoint;
			if(looking)
			{
				_lockedIn = true;
				_orientation = _targetEyePoint - _position;
			}
			if(_sequenceStarted && _currentWaypoint == nullptr && _waypoints.empty() && _onSequenceFinished)
			{
				_onSequenceFinished(this);
				_sequenceStarted = false;
			}
		}
		else
		{
			glm::vec3 delta = _targetEyePoint - _eyePoint;
			float len = std::max(0.5f, glm::length(delta));
			delta = glm::normalize(delta) * len * dt * 2.0f;
			_eyePoint += delta;
		}
		view = glm::lookAt(_position, _eyePoint, _up);
	}
	_matrix = _projection * view;
	_rotationMatrix = _projection * glm::mat4(glm::mat3(view));
	updateShaders();
}