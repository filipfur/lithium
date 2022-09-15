#pragma once
#include <glad/glad.h>
#include "glbuffer.h"

namespace lithium
{
	class VertexArray
	{
	public:
		VertexArray()
		{
			glGenVertexArrays(1, &_id);
		}

		~VertexArray() noexcept
		{
			glDeleteVertexArrays(1, &_id);
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

	private:
		GLuint _id;
	};
}