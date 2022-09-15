#include "glorthographiccamera.h"

lithium::OrthographicCamera::~OrthographicCamera() noexcept
{

}

void lithium::OrthographicCamera::update(float dt)
{
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), _position) * glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0, 0, 1));
	_view = glm::inverse(transform);
	_matrix = _projection * _view;
	updateShaders();
}