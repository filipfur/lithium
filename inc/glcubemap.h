#pragma once

#include "glelement.h"
#include "glmesh.h"
#include "glcamera.h"
#include "glshaderprogram.h"

namespace mygl
{
    class Cubemap : public Element
    {
        public:
            Cubemap(mygl::ShaderProgram* shaderProgram, mygl::Mesh* mesh, const std::string& fileName, const std::string& fileExt="png");
            virtual ~Cubemap() noexcept;

            virtual void bind() override;

            virtual void unbind() override;

            void draw(Camera* camera);

        private:
            mygl::Mesh* _mesh;
            mygl::ShaderProgram* _shaderProgram;
    };
}