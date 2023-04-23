#pragma once

#include "glbuffer.h"
#include <vector>

namespace lithium
{
    class VertexArrayBuffer : public Buffer
    {
    public:
        enum class AttributeType
		{
			FLOAT, VEC2, VEC3, VEC4, MAT3, MAT4
		};

		class AttributePointer2
		{
		public:
			AttributePointer2(GLuint components, GLenum type)
				: _components{components}, _type{type}
			{
				auto size = sizeof(GLuint);
				if(_type == GL_FLOAT)
				{
					size = sizeof(GLfloat);
				}
				else if(_type == GL_UNSIGNED_BYTE)
				{
					size = sizeof(GLubyte);
				}

				_size = components * size;
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

        VertexArrayBuffer(const VertexArrayBuffer& other)
			: Buffer{other}, _attributes{other._attributes}, _numLayouts{other._numLayouts}, _componentCount{other._componentCount},
			_layoutOffset{other._layoutOffset}, _attribDivisor{other._attribDivisor}, _componentType{other._componentType}
        {
            bind();
            linkAttributes();
			glEnableVertexAttribArray(0);
        }

        VertexArrayBuffer(const std::vector<AttributeType>& attributes, const std::vector<GLfloat>& vertices, GLenum usage=GL_STATIC_DRAW, GLuint layoutOffset=0, GLuint attribDivisor=0, GLuint componentType=GL_FLOAT)
            : Buffer{GL_ARRAY_BUFFER, usage}, _attributes{attributes}, _layoutOffset{layoutOffset}, _attribDivisor{attribDivisor}, _componentType{componentType}
        {
			allocate(vertices);
            bind();
            linkAttributes();
			glEnableVertexAttribArray(0);
        }

		VertexArrayBuffer(const std::vector<AttributeType>& attributes, const std::vector<GLuint>& vertices, GLenum usage=GL_STATIC_DRAW, GLuint layoutOffset=0, GLuint attribDivisor=0, GLuint componentType=GL_UNSIGNED_INT)
            : Buffer{GL_ARRAY_BUFFER, usage}, _attributes{attributes}, _layoutOffset{layoutOffset}, _attribDivisor{attribDivisor}, _componentType{componentType}
        {
			allocate(vertices);
            bind();
            linkAttributes();
			glEnableVertexAttribArray(0);
        }

		VertexArrayBuffer(const std::vector<AttributeType>& attributes, const std::vector<GLushort>& vertices, GLenum usage=GL_STATIC_DRAW, GLuint layoutOffset=0, GLuint attribDivisor=0, GLuint componentType=GL_UNSIGNED_SHORT)
            : Buffer{GL_ARRAY_BUFFER, usage}, _attributes{attributes}, _layoutOffset{layoutOffset}, _attribDivisor{attribDivisor}, _componentType{componentType}
        {
			allocate(vertices);
            bind();
            linkAttributes();
			glEnableVertexAttribArray(0);
        }

		VertexArrayBuffer(const std::vector<AttributeType>& attributes, const std::vector<GLubyte>& vertices, GLenum usage=GL_STATIC_DRAW, GLuint layoutOffset=0, GLuint attribDivisor=0, GLuint componentType=GL_UNSIGNED_BYTE)
            : Buffer{GL_ARRAY_BUFFER, usage}, _attributes{attributes}, _layoutOffset{layoutOffset}, _attribDivisor{attribDivisor}, _componentType{componentType}
        {
			allocate(vertices);
            bind();
            linkAttributes();
			glEnableVertexAttribArray(0);
        }
        
        virtual VertexArrayBuffer* clone() const override
        {
            return new VertexArrayBuffer(*this);
        }

        virtual ~VertexArrayBuffer() noexcept
        {

        }

        std::vector<AttributeType> attributes() const
        {
            return _attributes;
        }

        void linkAttribPointer(GLuint layout, GLuint numComponents, GLenum type, GLsizei stride, void* offset)
		{
			glEnableVertexAttribArray(layout + _layoutOffset);
			if(_componentType == GL_FLOAT)
			{
				glVertexAttribPointer(layout + _layoutOffset, numComponents, type, GL_FALSE, stride, offset);
			}
			else
			{
				glVertexAttribIPointer(layout + _layoutOffset, numComponents, type, stride, offset);
			}
			if(_attribDivisor > 0)
			{
				glVertexAttribDivisor(layout + _layoutOffset, _attribDivisor);
			}
		}

		void linkAttributes(const std::vector<AttributePointer2>& attribPtrs)
		{
			GLuint stride = 0;
			GLsizei offset = 0;
			std::for_each(attribPtrs.begin(), attribPtrs.end(), [&stride](AttributePointer2 attribPtr){
				stride += attribPtr.size();
			});
			for(int i{0}; i < attribPtrs.size(); ++i)
            {
				const AttributePointer2& attribPtr = attribPtrs.at(i);
                linkAttribPointer(i, attribPtr.components(), attribPtr.type(), stride, (void*)(intptr_t) offset);
				offset += attribPtr.size();
				_componentCount += attribPtr.components();
            }
			_numLayouts = attribPtrs.size();
		}

        void linkAttributes()
		{
			std::vector<AttributePointer2> aPtrs; // TODO: Move to GL_ARRAY_BUFFER
			for(AttributeType attribute : _attributes)
			{
				switch(attribute)
				{
					case AttributeType::FLOAT:
						aPtrs.push_back(AttributePointer2{1, _componentType});
						break;
					case AttributeType::VEC2:
						aPtrs.push_back(AttributePointer2{2, _componentType});
						break;
					case AttributeType::VEC3:
						aPtrs.push_back(AttributePointer2{3, _componentType});
						break;
					case AttributeType::VEC4:
						aPtrs.push_back(AttributePointer2{4, _componentType});
						break;
					case AttributeType::MAT3:
						aPtrs.push_back(AttributePointer2{3, _componentType});
						aPtrs.push_back(AttributePointer2{3, _componentType});
						aPtrs.push_back(AttributePointer2{3, _componentType});
						break;
					case AttributeType::MAT4:
						aPtrs.push_back(AttributePointer2{4, _componentType});
						aPtrs.push_back(AttributePointer2{4, _componentType});
						aPtrs.push_back(AttributePointer2{4, _componentType});
						aPtrs.push_back(AttributePointer2{4, _componentType});
						break;
				}
			}
			linkAttributes(aPtrs);
		}

		GLuint numLayouts() const
		{
			return _numLayouts;
		}

        GLuint componentCount() const
        {
            return _componentCount;
        }

    private:
        std::vector<AttributeType> _attributes;
        GLuint _numLayouts{0};
		GLuint _componentCount{0};
		GLuint _layoutOffset;
		GLuint _attribDivisor;
		GLuint _componentType;
    };
}