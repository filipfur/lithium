#include "glcamera.h"

lithium::Camera::Camera(const glm::mat4& projection, const glm::vec3& position, const glm::vec3& orientation, const glm::vec3& up)
	: _projection{projection},
	_view{glm::inverse(glm::translate(glm::mat4(1.0f), _position) * glm::rotate(glm::mat4(1.0f), 0.0f, _up))},
	_matrix{_projection * _view},
	_position{ position },
	_orientation{orientation},
	_up{up}
{
	
}