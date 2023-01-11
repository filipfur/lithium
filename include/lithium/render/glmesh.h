#pragma once

#include <vector>
#include <algorithm>

#include "glvertexarray.h"
#include "glbuffer.h"
#include <glm/gtc/type_ptr.hpp>

namespace lithium
{
	class Mesh : public VertexArray
	{
	public:

		Mesh(const Mesh& other) : VertexArray{other}
		{
		}

		Mesh(const std::vector<AttributeType>& attributes,
			const std::vector<GLfloat>& vertices)
			: VertexArray(DrawFunction::ARRAYS, attributes, vertices)
		{
			
		}

		Mesh(const std::vector<AttributeType>& attributes,
			const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
			: VertexArray{DrawFunction::ELEMENTS, attributes, vertices, indices}
		{
		}

		virtual ~Mesh() noexcept
		{
		}

		lithium::Mesh* clone()
		{
			return new lithium::Mesh(*this);
		}

	};
}