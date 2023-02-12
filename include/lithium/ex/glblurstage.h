#pragma once

#include "glrenderstage.h"
#include "gltexture.h"
#include "glmesh.h"

namespace lithium
{
    class ExBlurStage : public RenderStage
    {
    public:
        ExBlurStage(std::shared_ptr<lithium::FrameBuffer> frameBuffer, const glm::ivec4& viewport, std::shared_ptr<lithium::Mesh> mesh,
            std::shared_ptr<lithium::Texture<unsigned char>> inputTexture,
            std::shared_ptr<lithium::ShaderProgram> blurShader)
            : RenderStage{frameBuffer, viewport, [this](){ 
                    _mesh->bind();
                    _inputTexture->bind(GL_TEXTURE0);
                    _mesh->draw();
             }}, _resolution{inputTexture->width(), inputTexture->height()}, _mesh{mesh}, _inputTexture{inputTexture}, _blurShader{blurShader}
        {
            
        }

        virtual ~ExBlurStage() noexcept
        {

        }

        std::shared_ptr<lithium::Texture<unsigned char>> outputTexture()
        {
            return frameBuffer()->texture(GL_COLOR_ATTACHMENT0);
        }

        void setInputTexture(std::shared_ptr<lithium::Texture<unsigned char>> inputTexture)
        {
            _inputTexture = inputTexture;
        }

    private:
        const glm::ivec2 _resolution;
        std::shared_ptr<lithium::Mesh> _mesh{nullptr};
        bool _first{true};
        std::shared_ptr<lithium::Texture<unsigned char>> _inputTexture{nullptr};
        std::shared_ptr<lithium::ShaderProgram> _blurShader{nullptr};
    };
}