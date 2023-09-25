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
	class Mesh
	{
	public:

		using MaterialList = std::vector<std::shared_ptr<lithium::Material>>;
		using VertexArrayList = std::vector<std::shared_ptr<lithium::VertexArray>>;

		Mesh() : _materials{}, _vaos{}
		{

		}

		Mesh(VertexArray::DrawFunction drawFunction)
		{
			createVertexArray(drawFunction);
		}

		Mesh(const Mesh& other) : _materials{other._materials}
		{
			for(auto& vao : other._vaos)
			{
				_vaos.push_back(std::make_shared<lithium::VertexArray>(*vao));
			}
		}

		Mesh(const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
			const std::vector<GLfloat>& vertices)
		{
			_vaos.push_back(std::make_shared<VertexArray>(VertexArray::DrawFunction::ARRAYS, attributes, vertices));
		}

		Mesh(const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
			const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
		{
			_vaos.push_back(std::make_shared<VertexArray>(VertexArray::DrawFunction::ELEMENTS, attributes, vertices, indices));
		}

		virtual ~Mesh() noexcept
		{
		}

		lithium::Mesh* clone()
		{
			return new lithium::Mesh(*this);
		}

		void setMaterials(const MaterialList& materials)
		{
			_materials = materials;
		}

		std::shared_ptr<VertexArray> createVertexArray(VertexArray::DrawFunction drawFunction)
		{
			auto vao = std::make_shared<VertexArray>(drawFunction);
			_vaos.push_back(vao);
			return vao;
		}

		std::shared_ptr<lithium::Material> material(size_t index=0) const
		{
			return index < _materials.size() ? _materials.at(index) : nullptr;
		}

		std::shared_ptr<lithium::VertexArray> vertexArray(size_t index=0) const
		{
			return _vaos.at(index);
		}

		size_t materialCount() const
		{
			return _materials.size();
		}

		size_t vertexArrayCount() const
		{
			return _vaos.size();
		}

		void bind(size_t index = 0)
		{
			_vaos.at(index)->bind();
		}

		void draw(size_t index = 0)
		{
			_vaos.at(index)->draw();
		}

		void unbind()
		{
			_vaos.at(0)->unbind();
		}

		void appendMaterial(const std::shared_ptr<lithium::Material>& material)
		{
			_materials.push_back(material);
		}

	private:		
		MaterialList _materials;
		VertexArrayList _vaos;
	};
}