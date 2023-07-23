#pragma once

#include <vector>
#include <algorithm>
#include <memory>

#include <glm/gtc/type_ptr.hpp>
#include "glvertexarray.h"
#include "glbuffer.h"
#include "glmaterial.h"

namespace lithium
{
	class Mesh : public VertexArray
	{
	public:

		Mesh(DrawFunction drawFunction) : VertexArray{drawFunction}
		{
			
		}

		Mesh(const Mesh& other) : VertexArray{other}, _material{other._material}
		{
		}

		Mesh(const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
			const std::vector<GLfloat>& vertices)
			: VertexArray(DrawFunction::ARRAYS, attributes, vertices)
		{
			
		}

		Mesh(const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
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

		void setMaterial(std::shared_ptr<lithium::Material> material)
		{
			_material = material;
		}

		std::shared_ptr<lithium::Material> material() const
		{
			return _material;
		}
	private:		
		std::shared_ptr<lithium::Material> _material{nullptr};
	};
}