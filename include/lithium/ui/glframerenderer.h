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

        SimpleCamera& camera()
        {
            return _camera;
        }

        std::shared_ptr<lithium::ExTextRenderer> textRenderer()
        {
            return _textRenderer;
        }

        std::shared_ptr<lithium::ExTextRenderer> createTextRenderer()
        {
            _textRenderer = std::make_shared<lithium::ExTextRenderer>(_dimension);
            return _textRenderer;
        }

        lithium::Object::TexturePointer cachedTexture()
        {
            return _textureFBO->texture(GL_COLOR_ATTACHMENT0);
        }

        void invalidate()
        {
            _synced = false;
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
        std::shared_ptr<lithium::ExTextRenderer> _textRenderer{nullptr};
        bool _synced{false};
    };
}