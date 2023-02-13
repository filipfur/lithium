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

		Camera(const glm::mat4& projection, const glm::vec3& position, const glm::vec3& orientation, const glm::vec3& up);

		//virtual ~Camera() noexcept = 0;

		glm::mat4 matrix() const
		{
			return _matrix;
		}

		glm::vec3 position() const
		{
			return _position;
		}

		void setPosition(const glm::vec3& position)
		{
			_position = position;
		}

		glm::mat4 view() const
		{
			return _view;
		}

		glm::mat4 projection() const
		{
			return _projection;
		}

	protected:
		glm::vec3 _position{0.0f};
		glm::vec3 _orientation;
		glm::vec3 _up;
		glm::mat4 _projection;
		glm::mat4 _view;
		glm::mat4 _matrix;
	};
}
