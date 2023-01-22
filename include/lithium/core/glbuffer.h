#pragma once
#include <initializer_list>
#include <glad/glad.h>
#include <vector>
#include "glelement.h"

namespace lithium
{
	//template <typename T, GLuint S>
	class Buffer : public Element
	{
	public:

		Buffer(GLenum type, GLenum usage=GL_STATIC_DRAW) : _type{type}, _usage{usage}
		{
			glGenBuffers(1, &_id);
			_usage = usage;
		}

		Buffer(const Buffer& other) : _type{other._type}, _byteLength{other._byteLength}, _count{other._count}, _usage{other._usage}
		{
			glGenBuffers(1, &_id);
			bind();
			glBufferData(_type, _byteLength, 0, _usage);
			glBindBuffer(GL_COPY_READ_BUFFER, other._id);
			glBindBuffer(GL_COPY_WRITE_BUFFER, _id);
			glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, _byteLength); // ? / sizeof(T)
		}

		virtual Buffer* clone() const
		{
			return new Buffer(*this);
		}

		~Buffer() noexcept
		{
			glDeleteBuffers(1, &_id);
		}

		template <typename T>
		void allocate(const std::vector<T>& data, GLenum usage=GL_STATIC_DRAW)
		{
			_byteLength = data.size() * sizeof(T);
			_count = data.size();
			_usage = usage;
			bind();
			glBufferData(_type, _byteLength, data.data(), usage);
		}

		virtual void bind() override
		{
			glBindBuffer(_type, _id);
		}

		virtual void unbind() override
		{
			glBindBuffer(_type, 0);
		}

		GLenum type() const
		{
			return _type;
		}

		GLuint count()
		{
			return _count;
		}

	private:
		GLenum _type{};
		GLuint _byteLength{0};
		GLuint _count{0};
		GLenum _usage{};
	};
}