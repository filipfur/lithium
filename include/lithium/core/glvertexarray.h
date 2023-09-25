#pragma once
#include <memory>
#include <glad/glad.h>
#include "glbuffer.h"
#include "glvertexarraybuffer.h"

namespace lithium
{
	class VertexArray
	{
	public:
		enum class DrawFunction
		{
			ARRAYS,
			ELEMENTS,
			ELEMENTS16,
			ELEMENTS_INSTANCED,
			ELEMENTS16_INSTANCED
		};

		VertexArray(DrawFunction drawFunction) : _drawFunction{drawFunction}
		{
			glGenVertexArrays(1, &_id);
		}

		VertexArray(DrawFunction drawFunction, const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes)
			: _drawFunction{drawFunction}
		{
			glGenVertexArrays(1, &_id);
		}

		VertexArray(DrawFunction drawFunction, const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
			const std::vector<GLfloat>& vertices)
			: VertexArray{drawFunction, attributes}
		{
			bind();
			auto vao = createArrayBuffer(attributes, vertices);
			//_vertexArray->unbind(); TODO: Uncomment me when things have settled.
			vao->unbind();
		}

		VertexArray(DrawFunction drawFunction, const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
			const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
			: VertexArray{drawFunction, attributes}
		{
			bind();
			auto vao = createArrayBuffer(attributes, vertices);
			createElementArrayBuffer(indices);
			unbind();
			vao->unbind();
			_elementArrayBuffer->unbind();
		}

		VertexArray(const VertexArray& other) : VertexArray{other._drawFunction}
		{
			bind();
			_drawMode = other._drawMode;
			for(auto vao : other._vertexArrayBuffers)
			{
				_vertexArrayBuffers.push_back(std::shared_ptr<VertexArrayBuffer>(vao->clone()));
			}
			if(other._elementArrayBuffer) _elementArrayBuffer = std::shared_ptr<Buffer>(other._elementArrayBuffer->clone());
			//if(_elementArrayBuffer) _elementArrayBuffer->bind();
			//if(_elementArrayBuffer16) _elementArrayBuffer16->bind();

			unbind();
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			if(_elementArrayBuffer) _elementArrayBuffer->unbind();
		}

		~VertexArray() noexcept
		{
			_vertexArrayBuffers.clear();
			_elementArrayBuffer = nullptr;
			glDeleteVertexArrays(1, &_id);
		}

		template <typename T=GLfloat>
		std::shared_ptr<VertexArrayBuffer> createArrayBuffer(const std::vector<lithium::VertexArrayBuffer::AttributeType>& attributes,
			const std::vector<T>& vertices, GLuint componentType=GL_FLOAT)
		{
			int attribDivisor = 0;
			int layoutOffset = 0;
			int size = vertices.size();
			for(auto vao : _vertexArrayBuffers)
			{
				layoutOffset += vao->numLayouts();
			}
			auto vertexArrayBuffer = std::make_shared<VertexArrayBuffer>(attributes, vertices, GL_STATIC_DRAW, layoutOffset, attribDivisor, componentType);
			_vertexArrayBuffers.push_back(vertexArrayBuffer);
			return vertexArrayBuffer;
		}

		std::shared_ptr<lithium::Buffer> createElementArrayBuffer(const std::vector<GLuint>& indices)
		{
			_elementArrayBuffer = std::make_shared<lithium::Buffer>(GL_ELEMENT_ARRAY_BUFFER);
			if(indices.size() > 0)
			{
				_elementArrayBuffer->allocate(indices);
			}
			return _elementArrayBuffer;
		}

		std::shared_ptr<lithium::Buffer> createElementArrayBuffer(const std::vector<GLushort>& indices)
		{
			_elementArrayBuffer = std::make_shared<lithium::Buffer>(GL_ELEMENT_ARRAY_BUFFER);
			if(indices.size() > 0)
			{
				_elementArrayBuffer->allocate(indices);
			}
			return _elementArrayBuffer;
		}

		void bind()
		{
			glBindVertexArray(_id);
		}

		void unbind()
		{
			glBindVertexArray(0);
		}

		std::shared_ptr<lithium::VertexArrayBuffer> vertexArrayBuffer(size_t index) const
		{
			return _vertexArrayBuffers[index];
		}

		std::shared_ptr<lithium::Buffer> elementArrayBuffer() const
		{
			return _elementArrayBuffer;
		}

		void draw()
		{
			switch(_drawFunction)
			{
				case DrawFunction::ARRAYS:
					glDrawArrays(_drawMode, 0, _vertexArrayBuffers[0]->count());
					break;
				case DrawFunction::ELEMENTS:
					glDrawElements(_drawMode, static_cast<GLuint>(_elementArrayBuffer->count()), GL_UNSIGNED_INT, 0);
					break;
				case DrawFunction::ELEMENTS16:
					glDrawElements(_drawMode, static_cast<GLuint>(_elementArrayBuffer->count()), GL_UNSIGNED_SHORT, 0);
					break;
				case DrawFunction::ELEMENTS_INSTANCED:
					glDrawElementsInstanced(_drawMode, static_cast<GLuint>(_elementArrayBuffer->count()), GL_UNSIGNED_INT, 0, _instanceCount);
					break;
				case DrawFunction::ELEMENTS16_INSTANCED:
					glDrawElementsInstanced(_drawMode, static_cast<GLuint>(_elementArrayBuffer->count()), GL_UNSIGNED_SHORT, 0, _instanceCount);
					break;
			}
		}

		void setDrawMode(GLenum drawMode)
		{
			_drawMode = drawMode;
		}

		void setDrawFunction(DrawFunction drawFunction)
		{
			_drawFunction = drawFunction;
		}

		DrawFunction drawFunction() const
		{
			return _drawFunction;
		}

		void setInstanceCount(GLuint instanceCount)
		{
			_instanceCount = instanceCount;
		}

		size_t vertexArrayBufferCount() const
		{
			return _vertexArrayBuffers.size();
		}

	private:

		GLuint _id;
		DrawFunction _drawFunction;
		GLenum _drawMode{GL_TRIANGLES};
		std::vector<std::shared_ptr<lithium::VertexArrayBuffer>> _vertexArrayBuffers;
		std::shared_ptr<lithium::Buffer> _elementArrayBuffer{nullptr};
		GLuint _instanceCount;
	};
}