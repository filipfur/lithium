#include "glperspectivecamera.h"


lithium::PerspectiveCamera::~PerspectiveCamera() noexcept
{

}

void lithium::PerspectiveCamera::update(float dt)
{
	lithium::Camera::update(dt);
	bool looking = false;

	if(_lockedIn)
	{
		_view = glm::lookAt(_position, _position + _orientation, _up);
	}
	else
	{
		if(_currentWaypoint != nullptr)
		{
			if(_currentWaypoint->hasOrientation())
			{
				_targetViewPoint = _position + _currentWaypoint->orientation();
				_lockedIn = true;
				_orientation = _currentWaypoint->orientation();
			}
			else
			{
				_targetViewPoint = _currentWaypoint->position();
			}
		}
		else if(_lookingAt != nullptr)
		{
			_targetViewPoint = *_lookingAt;
			looking = true;
		}
		else
		{
			_targetViewPoint = _position + _orientation;
		}
		if(glm::distance2(_viewPoint, _targetViewPoint) < 0.0001)
		{
			_viewPoint = _targetViewPoint;
			if(looking)
			{
				_lockedIn = true;
				_orientation = _targetViewPoint - _position;
			}
			if(_sequenceStarted && _currentWaypoint == nullptr && _waypoints.empty() && _onSequenceFinished)
			{
				_onSequenceFinished(this);
				_sequenceStarted = false;
			}
		}
		else
		{
			glm::vec3 delta = _targetViewPoint - _viewPoint;
			float len = std::max(0.5f, glm::length(delta));
			delta = glm::normalize(delta) * len * dt * 2.0f;
			_viewPoint += delta;
		}
		_view = glm::lookAt(_position, _viewPoint, _up);
	}
	_matrix = _projection * _view;
	_rotationMatrix = _projection * glm::mat4(glm::mat3(_view));
	updateShaders();
}