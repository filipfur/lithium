#include "glrenderstage.h"


lithium::RenderStage::RenderStage(std::shared_ptr<lithium::FrameBuffer> frameBuffer, const glm::ivec4& viewport, const CallbackType& callback)
    : _frameBuffer{frameBuffer}, _viewport{viewport}, _callback{callback}
{

}

lithium::RenderStage::~RenderStage() noexcept
{

}

void lithium::RenderStage::render()
{
    if(_frameBuffer) { _frameBuffer->bind(); }
    if(_viewport != latestViewport)
    {
        glViewport(_viewport.x, _viewport.y, _viewport.z, _viewport.w);
        latestViewport = _viewport;
    }
    _callback();
    if(_frameBuffer) { _frameBuffer->unbind(); }
}

glm::ivec4 lithium::RenderStage::latestViewport{0};