#pragma once

#include "globject.h"
#include "glattributepointer.h"

namespace lithium
{
    template <class T>
    class InstancedObject : public Object
    {
    public:
        InstancedObject(std::shared_ptr<lithium::Mesh> mesh, const std::vector<lithium::Object::TexturePointer>& textures)
            : lithium::Object{mesh, textures}, _instancedArray{GL_ARRAY_BUFFER}
        {
            lithium::VertexArray::DrawFunction nextDrawFunc;
            switch(mesh->vertexArray()->drawFunction())
            {
                case lithium::VertexArray::DrawFunction::ELEMENTS16:
                    nextDrawFunc = lithium::VertexArray::DrawFunction::ELEMENTS16_INSTANCED;
                    break;
                case lithium::VertexArray::DrawFunction::ELEMENTS:
                    nextDrawFunc = lithium::VertexArray::DrawFunction::ELEMENTS_INSTANCED;
                    break;
                default:
                    std::cerr << "error: instancedobject does not support draw function "
                        << static_cast<int>(mesh->vertexArray()->drawFunction()) << std::endl;
                    exit(1);
                    break;
            }
            mesh->vertexArray()->setDrawFunction(nextDrawFunc);
        }

        InstancedObject(const InstancedObject& other)
            : lithium::Object{other}, _instancedArray{GL_ARRAY_BUFFER}
        {
            setMesh(std::make_shared<Mesh>(*other._mesh));    
        }

        InstancedObject(const Object& other) : lithium::Object{other}, _instancedArray{GL_ARRAY_BUFFER}
        {

        }

        virtual ~InstancedObject() noexcept
        {
            _instances.clear();
        }

        void addInstance(T instanceMatrix)
        {
            _instances.push_back(instanceMatrix);
        }

        void setInstances(const std::vector<T>& instances)
        {
            _instances = instances;
        }

        void allocateBufferData()
        {
            _mesh->bind();
            _instancedArray.allocate(_instances);
            _mesh->vertexArray()->setInstanceCount(_instances.size());
            _mesh->unbind();
        }

        template <GLenum U=GL_FLOAT>
        void linkBuffer(std::vector<lithium::AttributePointer<U>> attribPtrs)
        {
            _mesh->bind();
            //_instancedArray.bind();
            int n = 0;

            for(size_t i{0}; i < _mesh->vertexArray()->vertexArrayBufferCount(); ++i)
            {
                n += _mesh->vertexArray()->vertexArrayBuffer(i)->numLayouts();
            }

            if(_mesh->vertexArrayCount() > 1)
            {
                std::cerr << "error: instacnedobject with multiple VAOs are not supported" << std::endl;
                exit(1);
            }

            for(auto && attribPtr : attribPtrs)
            {
                for(size_t i{0}; i < _mesh->vertexArray()->vertexArrayBufferCount(); ++i)
                {
                    _mesh->vertexArray()->vertexArrayBuffer(i)->linkAttribPointer(n + attribPtr.layout(), attribPtr.numComponents(), attribPtr.type(), attribPtr.stride(), attribPtr.offset());
                }
                glVertexAttribDivisor(n + attribPtr.layout(), 1);
            }

            //_instancedArray.unbind();
            _mesh->unbind();
        }

    protected:
        std::vector<T> _instances;
        lithium::Buffer _instancedArray;
    };
}