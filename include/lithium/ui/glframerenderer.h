#pragma once

#include "glrenderpipeline.h"
#include "glsimplecamera.h"
#include "gluniformbufferobject.h"
#include "ex/gltextrenderer.h"

namespace lithium
{
    class FrameRenderer : public RenderPipeline
    {
    public:
        FrameRenderer(const glm::vec2& dimension);

        virtual ~FrameRenderer() noexcept;

        glm::vec2 resolution() const
        {
            return _resolution;
        }

        SimpleCamera& camera()
        {
            return _camera;
        }

        lithium::ExTextRenderer& textRenderer()
        {
            return _textRenderer;
        }

        lithium::Object::TexturePointer cachedTexture()
        {
            return _textureFBO->texture(GL_COLOR_ATTACHMENT0);
        }

    private:
        glm::vec2 _dimension;
        SimpleCamera _camera;
        UniformBufferObject _canvasUBO;
        std::shared_ptr<FrameBuffer> _msaaFBO;
        std::shared_ptr<FrameBuffer> _textureFBO;
        std::shared_ptr<ShaderProgram> _msaaShader;
        std::shared_ptr<ShaderProgram> _frameShader;
        std::shared_ptr<RenderGroup> _frameRenderGroup;
        std::shared_ptr<lithium::RenderStage> _mainStage;
        std::shared_ptr<lithium::RenderStage> _downSampleStage;
        std::shared_ptr<RenderStage> _finalStage;
        lithium::ExTextRenderer _textRenderer;
        bool _synced{false};
    };
}