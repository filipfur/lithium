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
            : lithium::Object{mesh, diffuse, normal}, _instancedArray{}
        {
            
        }

        InstancedObject(const InstancedObject& other)
            : lithium::Object{other}, _instancedArray{}
        {
            setMesh(new Mesh(*other._mesh));    
        }

        InstancedObject(const Object& other) : lithium::Object{other}, _instancedArray{}
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
            _mesh->bindVertexArray();
            _instancedArray.allocate(_instances, 0);
        }

        template <GLenum U=GL_FLOAT>
        void linkBuffer(std::vector<lithium::AttributePointer<U>> attribPtrs)
        {
            auto vao = _mesh->vertexArray();
            vao->bind();
            int n = _mesh->numLayouts();
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
                vao->linkAttribPointer(n + attribPtr.layout(), attribPtr.numComponents(), attribPtr.type(), attribPtr.stride(), attribPtr.offset());
                glVertexAttribDivisor(n + attribPtr.layout(), 1);
            }

            vao->unbind();
        }

        virtual void onDraw() override
        {
            //_mesh->drawElements();
            //_instancedArray.bind();
            int n = _instances.size();
            _mesh->drawElementsInstanced(_drawMax < n ? _drawMax : n);
        }

        void setDrawMax(int drawMax)
        {
            _drawMax = drawMax;
        }

    private:
        int _drawMax{INT_MAX};
        std::vector<T> _instances;
        lithium::Buffer<T, GL_ARRAY_BUFFER> _instancedArray;
    };
}