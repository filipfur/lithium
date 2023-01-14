#pragma once
#include <initializer_list>
#include <glad/glad.h>
#include <vector>
#include "glelement.h"

namespace lithium
{
	template <typename T, GLuint S>
	class Buffer : public Element
	{
	public:

		Buffer()
		{
			glGenBuffers(1, &_id);
		}

		Buffer(GLuint size, GLenum usage=GL_STATIC_DRAW) : Buffer{}
		{
			_size = size / sizeof(T);
			_usage = usage;
			bind();
			glBufferData(S, size, NULL, usage);
		}

		Buffer(const Buffer& other) : Buffer{}
		{
			bind();
			auto size = other._size * sizeof(T);
			_size = other._size;
			_usage = other._usage;
			glBufferData(S, _size, 0, usage);
			glBindBuffer(GL_COPY_READ_BUFFER, other._id);
			glBindBuffer(GL_COPY_WRITE_BUFFER, _id);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
		}

		Buffer(const std::vector<T>& data, GLenum usage=GL_STATIC_DRAW) : Buffer{}
		{
			allocate(data, usage);
		}

		virtual Buffer* clone() const
		{
			return new Buffer(*this);
		}

		~Buffer() noexcept
		{
			glDeleteBuffers(1, &_id);
		}

		void allocate(const std::vector<T>& data, GLenum usage=GL_STATIC_DRAW)
		{
			_size = data.size();
			_usage = usage;
			bind();
			glBufferData(S, _size * sizeof(T), data.data(), usage);
		}

		virtual void bind() override
		{
			glBindBuffer(S, _id);
		}

		virtual void unbind() override
		{
			glBindBuffer(S, 0);
		}

		GLuint size()
		{
			return _size;
		}

	private:
		GLuint _size{0};
		GLenum _usage{};
	};
}