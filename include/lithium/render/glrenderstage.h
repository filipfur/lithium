#pragma once

#include <memory>
#include <functional>
#include "glframebuffer.h"

namespace lithium
{
    class RenderStage
    {
    public:

        using CallbackType = std::function<void()>;

        RenderStage(std::shared_ptr<lithium::FrameBuffer> frameBuffer, const CallbackType& callback);

        virtual ~RenderStage() noexcept;

        virtual void render(const glm::ivec4& defaultViewport);

        std::shared_ptr<lithium::FrameBuffer> frameBuffer()
        {
            return _frameBuffer;
        }

        void setFrameBuffer(std::shared_ptr<lithium::FrameBuffer> frameBuffer)
        {
            _frameBuffer = frameBuffer;
        }

        void setEnabled(bool enabled)
        {
            _enabled = enabled;
        }

        bool enabled() const
        {
            return _enabled;
        }

    private:
        std::shared_ptr<lithium::FrameBuffer> _frameBuffer;
        CallbackType _callback;
        bool _enabled{true};
        static glm::ivec4 latestViewport;
    };
}