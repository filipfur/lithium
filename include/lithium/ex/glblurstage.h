#pragma once

#include "glrenderstage.h"
#include "gltexture.h"
#include "glmesh.h"

namespace lithium
{
    class ExBlurStage : public RenderStage
    {
    public:

        ExBlurStage(const glm::ivec4& viewport, std::shared_ptr<lithium::Mesh> mesh,
            std::shared_ptr<lithium::Texture<unsigned char>> inputTexture,
            std::shared_ptr<lithium::ShaderProgram> blurShader)
            : RenderStage{nullptr, viewport, [](){ return false; }}, _resolution{inputTexture->width(), inputTexture->height()}, _mesh{mesh}, _inputTexture{inputTexture}, _blurShader{blurShader}
        {
            _horizontalBlurFBO = std::make_shared<lithium::FrameBuffer>(_resolution, lithium::FrameBuffer::Mode::DEFAULT);
            _horizontalBlurFBO->bind();
                _horizontalBlurFBO->createTexture(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT);
                //_horizontalBlurFBO->createRenderBuffer(lithium::RenderBuffer::Mode::MULTISAMPLED, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
            _horizontalBlurFBO->unbind();

            _verticalBlurFBO = std::make_shared<lithium::FrameBuffer>(_resolution, lithium::FrameBuffer::Mode::DEFAULT);
            _verticalBlurFBO->bind();
                _verticalBlurFBO->createTexture(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT);
                //_verticalBlurFBO->createRenderBuffer(lithium::RenderBuffer::Mode::MULTISAMPLED, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
            _verticalBlurFBO->unbind();
        }

        virtual ~ExBlurStage() noexcept
        {

        }

        virtual void render() override
        {
            _blurShader->use();
            _blurShader->setResolution(glm::vec2(_resolution.x, _resolution.y));
            bool firstIteration{true};
            for(int i{0}; i < 5; ++i)
            {
                _blurShader->setUniform("u_horizontal", 0);
                _verticalBlurFBO->bind();
                {
                    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    _mesh->bind();
                    if(firstIteration)
                    {
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        _inputTexture->bind(GL_TEXTURE0);
                        //firstIteration = false;
                    }
                    else
                    {
                        _horizontalBlurFBO->bindTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE0);
                    }
                    _mesh->draw();
                }
                _verticalBlurFBO->unbind();
                _blurShader->setUniform("u_horizontal", 1);
                _horizontalBlurFBO->bind();
                {
                    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                    if(firstIteration)
                    {
                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        firstIteration = false;
                    }
                    _mesh->bind();
                    _verticalBlurFBO->bindTexture(GL_COLOR_ATTACHMENT0, GL_TEXTURE0);
                    _mesh->draw();
                }
                _horizontalBlurFBO->unbind();
            }
        }

        std::shared_ptr<lithium::Texture<unsigned char>> outputTexture()
        {
            return _horizontalBlurFBO->texture(GL_COLOR_ATTACHMENT0);
        }

    private:
        const glm::ivec2 _resolution;
        std::shared_ptr<lithium::Mesh> _mesh{nullptr};
        std::shared_ptr<lithium::Texture<unsigned char>> _inputTexture{nullptr};
        std::shared_ptr<lithium::FrameBuffer> _horizontalBlurFBO{nullptr};
        std::shared_ptr<lithium::FrameBuffer> _verticalBlurFBO{nullptr};
        std::shared_ptr<lithium::ShaderProgram> _blurShader{nullptr};
    };
}