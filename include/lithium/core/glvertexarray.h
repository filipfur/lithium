#pragma once
#include <glad/glad.h>
#include "glbuffer.h"

namespace lithium
{
	class VertexArray
	{
	public:
		enum class DrawFunction
		{
			ARRAYS,
			ELEMENTS,
			ELEMENTS_INSTANCED
		};

		enum class AttributeType
		{
			FLOAT, VEC2, VEC3, VEC4, MAT3, MAT4
		};

		class AttributePointer2
		{
		public:
			AttributePointer2(GLuint components, GLenum type) : _components{components}, _type{type}
			{
				_size = components * (type == GL_FLOAT ? sizeof(float) : sizeof(int));
			}

			virtual ~AttributePointer2() noexcept
			{

			}

			GLuint components() const
			{
				return _components;
			}

			GLenum type() const
			{
				return _type;
			}

			GLuint size() const
			{
				return _size;
			}

		private:
			GLuint _components;
			GLenum _type;
			GLuint _size;
		};

		VertexArray(DrawFunction drawFunction, const std::vector<AttributeType>& attributes)
			: _drawFunction{drawFunction}, _attributes{attributes}
		{
			glGenVertexArrays(1, &_id);
		}

		VertexArray(DrawFunction drawFunction, const std::vector<AttributeType>& attributes,
			const std::vector<GLfloat>& vertices)
			: VertexArray{drawFunction, attributes}
		{
			bind();
			_vertexArrayBuffer = new lithium::Buffer<GLfloat, GL_ARRAY_BUFFER>(vertices);
			linkAttributes(attributes);
			glEnableVertexAttribArray(0);
			//_vertexArray->unbind(); TODO: Uncomment me when things have settled.
			_vertexArrayBuffer->unbind();
		}

		VertexArray(DrawFunction drawFunction, const std::vector<AttributeType>& attributes,
			const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices)
			: VertexArray{drawFunction, attributes}
		{
			bind();
			_vertexArrayBuffer = new lithium::Buffer<GLfloat, GL_ARRAY_BUFFER>(vertices);
			linkAttributes(attributes);
			_elementArrayBuffer = new lithium::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>(indices);
			unbind();
			_vertexArrayBuffer->unbind();
			_elementArrayBuffer->unbind();
		}

		VertexArray(const VertexArray& other) : VertexArray{other._drawFunction, other._attributes}
		{
			bind();
			_drawMode = other._drawMode;
			_vertexArrayBuffer = new lithium::Buffer<GLfloat, GL_ARRAY_BUFFER>(*other._vertexArrayBuffer);
			
			if(other._elementArrayBuffer) _elementArrayBuffer = new lithium::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>(*other._elementArrayBuffer);
			_vertexArrayBuffer->bind();
			linkAttributes(_attributes);
			if(_elementArrayBuffer) _elementArrayBuffer->bind();

			unbind();
			_vertexArrayBuffer->unbind();
			if(_elementArrayBuffer) _elementArrayBuffer->unbind();
		}

		~VertexArray() noexcept
		{
			delete _vertexArrayBuffer;
			delete _elementArrayBuffer;
			glDeleteVertexArrays(1, &_id);
		}

		lithium::Buffer<GLfloat, GL_ARRAY_BUFFER>* createArrayBuffer()
		{
			_vertexArrayBuffer = new lithium::Buffer<GLfloat, GL_ARRAY_BUFFER>();
			return _vertexArrayBuffer;
		}

		lithium::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>* createElementArrayBuffer()
		{
			_elementArrayBuffer = new lithium::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>();
			return _elementArrayBuffer;
		}

		void linkAttribPointer(GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset)
		{
			glEnableVertexAttribArray(layout);
			glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
		}

		void bind()
		{
			glBindVertexArray(_id);
		}

		void unbind()
		{
			glBindVertexArray(0);
		}

		lithium::Buffer<GLfloat, GL_ARRAY_BUFFER>* vertexArrayBuffer() const
		{
			return _vertexArrayBuffer;
		}

		lithium::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>* elementArrayBuffer() const
		{
			return _elementArrayBuffer;
		}

		void draw()
		{
			switch(_drawFunction)
			{
				case DrawFunction::ARRAYS:
					glDrawArrays(_drawMode, 0, _count);
					break;
				case DrawFunction::ELEMENTS:
					glDrawElements(_drawMode, static_cast<GLuint>(_elementArrayBuffer->size()), GL_UNSIGNED_INT, 0);
					break;
				case DrawFunction::ELEMENTS_INSTANCED:
					glDrawElementsInstanced(_drawMode, static_cast<GLuint>(_elementArrayBuffer->size()), GL_UNSIGNED_INT, 0, _instanceCount);
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

		void linkAttributes(const std::vector<AttributePointer2>& attribPtrs)
		{
			GLuint stride = 0;
			int offset = 0;
			std::for_each(attribPtrs.begin(), attribPtrs.end(), [&stride](AttributePointer2 attribPtr){
				stride += attribPtr.size();
			});
			for(int i{0}; i < attribPtrs.size(); ++i)
            {
				const AttributePointer2& attribPtr = attribPtrs.at(i);
                linkAttribPointer(i, attribPtr.components(), attribPtr.type(), stride, (void*) offset);
				offset += attribPtr.size();
				_count += attribPtr.components();
            }
			_numLayouts = attribPtrs.size();
		}

		void linkAttributes(const std::vector<AttributeType>& attributes)
		{
			std::vector<AttributePointer2> aPtrs; // TODO: Move to GL_ARRAY_BUFFER
			for(AttributeType attribute : attributes)
			{
				switch(attribute)
				{
					case AttributeType::FLOAT:
						aPtrs.push_back(AttributePointer2{1, GL_FLOAT});
						break;
					case AttributeType::VEC2:
						aPtrs.push_back(AttributePointer2{2, GL_FLOAT});
						break;
					case AttributeType::VEC3:
						aPtrs.push_back(AttributePointer2{3, GL_FLOAT});
						break;
					case AttributeType::VEC4:
						aPtrs.push_back(AttributePointer2{4, GL_FLOAT});
						break;
					case AttributeType::MAT3:
						aPtrs.push_back(AttributePointer2{3, GL_FLOAT});
						aPtrs.push_back(AttributePointer2{3, GL_FLOAT});
						aPtrs.push_back(AttributePointer2{3, GL_FLOAT});
						break;
					case AttributeType::MAT4:
						aPtrs.push_back(AttributePointer2{4, GL_FLOAT});
						aPtrs.push_back(AttributePointer2{4, GL_FLOAT});
						aPtrs.push_back(AttributePointer2{4, GL_FLOAT});
						aPtrs.push_back(AttributePointer2{4, GL_FLOAT});
						break;
				}
			}
			linkAttributes(aPtrs);
		}

		GLuint numLayouts() const
		{
			return _numLayouts;
		}

		void setInstanceCount(GLuint instanceCount)
		{
			_instanceCount = instanceCount;
		}

	private:

		GLuint _id;
		DrawFunction _drawFunction;
		std::vector<AttributeType> _attributes;
		GLenum _drawMode{GL_TRIANGLES};
		lithium::Buffer<GLfloat, GL_ARRAY_BUFFER>* _vertexArrayBuffer{nullptr};
		lithium::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>* _elementArrayBuffer{nullptr};
		GLuint _numLayouts{0};
		GLuint _count{0};
		GLuint _instanceCount;
	};
}