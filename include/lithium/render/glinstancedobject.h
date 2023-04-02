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
            mesh->setDrawFunction(lithium::VertexArray::DrawFunction::ELEMENTS_INSTANCED);
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
            _mesh->setInstanceCount(_instances.size());
            _mesh->unbind();
        }

        template <GLenum U=GL_FLOAT>
        void linkBuffer(std::vector<lithium::AttributePointer<U>> attribPtrs)
        {
            _mesh->bind();
            //_instancedArray.bind();
            int n = _mesh->vertexArrayBuffer(0)->numLayouts();

            for(auto && attribPtr : attribPtrs)
            {
                _mesh->vertexArrayBuffer(0)->linkAttribPointer(n + attribPtr.layout(), attribPtr.numComponents(), attribPtr.type(), attribPtr.stride(), attribPtr.offset());
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