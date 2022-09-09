#pragma once

#include <vector>

#include "glvertexarray.h"
#include "glbuffer.h"
#include <glm/gtc/type_ptr.hpp>

namespace mygl
{
	class Mesh
	{
	public:
		enum class State{
			POS_NORMAL_UV, POS_NORMAL_UV_TANGENTS, XYZW
		};

		Mesh(const Mesh& other)
		{
			_vertexArray = new VertexArray();
			_vertexArray->bind();
			_vertexArrayBuffer = new mygl::Buffer<GLfloat, GL_ARRAY_BUFFER>(*other._vertexArrayBuffer);
			_elementArrayBuffer = new mygl::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>(*other._elementArrayBuffer);
			_vertexArrayBuffer->bind();
			_elementArrayBuffer->bind();
			_state = other._state;
			setAttribPointer(_state);

			_vertexArray->unbind();
			_vertexArrayBuffer->unbind();
			_elementArrayBuffer->unbind();
		}

		Mesh(const std::vector<GLfloat>& vertices, const std::vector<GLuint>& indices, State state=State::POS_NORMAL_UV)
			: _vertexArray{ new VertexArray() }, _state{state}
		{
			_vertexArray->bind();

			_vertexArrayBuffer = new mygl::Buffer<GLfloat, GL_ARRAY_BUFFER>(vertices);
			_elementArrayBuffer = new mygl::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>(indices);

			setAttribPointer(state);

			_vertexArray->unbind();
			_vertexArrayBuffer->unbind();
			_elementArrayBuffer->unbind();
		}

		void setAttribPointer(State state)
		{
			switch(state)
			{
				case mygl::Mesh::State::POS_NORMAL_UV_TANGENTS:
					_vertexArray->linkAttribPointer(0, 3, GL_FLOAT, 14 * sizeof(float), (void*)0);
					_vertexArray->linkAttribPointer(1, 3, GL_FLOAT, 14 * sizeof(float), (void*)(3 * sizeof(float)));
					_vertexArray->linkAttribPointer(2, 2, GL_FLOAT, 14 * sizeof(float), (void*)(6 * sizeof(float)));
					_vertexArray->linkAttribPointer(3, 3, GL_FLOAT, 14 * sizeof(float), (void*)(8 * sizeof(float)));
					_vertexArray->linkAttribPointer(4, 3, GL_FLOAT, 14 * sizeof(float), (void*)(11 * sizeof(float)));
					_numLayouts = 5;
					break;
				case mygl::Mesh::State::POS_NORMAL_UV:
					_vertexArray->linkAttribPointer(0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
					_vertexArray->linkAttribPointer(1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
					_vertexArray->linkAttribPointer(2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
					_numLayouts = 3;
					break;
				case mygl::Mesh::State::XYZW:
					_vertexArray->linkAttribPointer(0, 4, GL_FLOAT, 4 * sizeof(float), (void*)0);
					_numLayouts = 1;
					break;
			}
		}

		virtual ~Mesh()
		{
			delete _vertexArray;
			delete _vertexArrayBuffer;
			delete _elementArrayBuffer;
		}

		void bindVertexArray()
		{
			_vertexArray->bind();
		}

		void drawElements()
		{
			glDrawElements(GL_TRIANGLES, static_cast<GLuint>(_elementArrayBuffer->size()), GL_UNSIGNED_INT, 0);
		}

		void drawElementsInstanced(int n)
		{
			glDrawElementsInstanced(GL_TRIANGLES, static_cast<GLuint>(_elementArrayBuffer->size()), GL_UNSIGNED_INT, 0, n);
		}

		mygl::VertexArray* vertexArray() const
		{
			return _vertexArray;
		}

		mygl::Buffer<GLfloat, GL_ARRAY_BUFFER>* vertexArrayBuffer() const
		{
			return _vertexArrayBuffer;
		}

		mygl::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>* elementArrayBuffer() const
		{
			return _elementArrayBuffer;
		}

		GLuint numLayouts() const 
		{
			return _numLayouts;
		}

	private:
		State _state;
		//std::vector<GLfloat> _vertices;
		GLuint _numLayouts{0};
		mygl::VertexArray* _vertexArray;
		mygl::Buffer<GLfloat, GL_ARRAY_BUFFER>* _vertexArrayBuffer;
		mygl::Buffer<GLuint, GL_ELEMENT_ARRAY_BUFFER>* _elementArrayBuffer;
	};
}