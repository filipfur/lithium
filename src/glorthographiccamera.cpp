#include "glorthographiccamera.h"

lithium::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float nearPlane, float farPlane)
: lithium::Camera{glm::ortho(left, right, bottom, top, nearPlane, farPlane),
	glm::vec3{0.0f},
	glm::vec3{0.0f},
	glm::vec3{0.0, 0.0, 1.0}}
{

	_matrix = _projection * _view;
}

lithium::OrthographicCamera::~OrthographicCamera() noexcept
{

}