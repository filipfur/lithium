#pragma once

#include "glelement.h"
#include "glmesh.h"
#include "glcamera.h"
#include "glshaderprogram.h"

namespace lithium
{
    class Cubemap : public Element
    {
        public:
            Cubemap(lithium::ShaderProgram* shaderProgram, lithium::Mesh* mesh, const std::string& fileName, const std::string& fileExt="png");
            virtual ~Cubemap() noexcept;

            virtual void bind() override;

            virtual void unbind() override;

            void draw(Camera* camera);

        private:
            lithium::Mesh* _mesh;
            lithium::ShaderProgram* _shaderProgram;
    };
}