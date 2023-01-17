#pragma once

#include "globject.h"
#include "glattributepointer.h"

namespace lithium
{
    template <class T>
    class InstancedObject : public Object
    {
    public:
        InstancedObject(lithium::Mesh* mesh, lithium::ImageTexture* diffuse=nullptr, lithium::ImageTexture* normal=nullptr)
            : lithium::Object{mesh, diffuse, normal}, _instancedArray{GL_ARRAY_BUFFER}
        {
            mesh->setDrawFunction(lithium::VertexArray::DrawFunction::ELEMENTS_INSTANCED);
        }

        InstancedObject(const InstancedObject& other)
            : lithium::Object{other}, _instancedArray{GL_ARRAY_BUFFER}
        {
            setMesh(new Mesh(*other._mesh));    
        }

        InstancedObject(const Object& other) : lithium::Object{other}, _instancedArray{GL_ARRAY_BUFFER}
        {

        }

        virtual ~InstancedObject() noexcept
        {
            _instances.clear();
            delete _mesh;
        }

        void addInstance(T instanceMatrix)
        {
            _instances.push_back(instanceMatrix);
        }

        void allocateBufferData()
        {
            _mesh->bind();
            _instancedArray.allocate(_instances);
            _mesh->setInstanceCount(_instances.size());
        }

        template <GLenum U=GL_FLOAT>
        void linkBuffer(std::vector<lithium::AttributePointer<U>> attribPtrs)
        {
            _mesh->bind();
            //_instancedArray.bind();
            int n = _mesh->vertexArrayBuffer(0)->numLayouts();
            /*vao->linkAttribPointer(n, 4, GL_FLOAT, sizeof(glm::mat4), (void*)0);
            vao->linkAttribPointer(n+1, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
            vao->linkAttribPointer(n+2, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
            vao->linkAttribPointer(n+3, 4, GL_FLOAT, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
            glVertexAttribDivisor(n, 1);
            glVertexAttribDivisor(n+1, 1);
            glVertexAttribDivisor(n+2, 1);
            glVertexAttribDivisor(n+3, 1);*/

            for(auto && attribPtr : attribPtrs)
            {
                _mesh->vertexArrayBuffer(0)->linkAttribPointer(n + attribPtr.layout(), attribPtr.numComponents(), attribPtr.type(), attribPtr.stride(), attribPtr.offset());
                glVertexAttribDivisor(n + attribPtr.layout(), 1);
            }

            _mesh->unbind();
        }

    protected:
        std::vector<T> _instances;
        lithium::Buffer _instancedArray;
    };
}