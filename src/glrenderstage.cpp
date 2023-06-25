#include "glrenderstage.h"


lithium::RenderStage::RenderStage(std::shared_ptr<lithium::FrameBuffer> frameBuffer, const CallbackType& callback)
    : _frameBuffer{frameBuffer}, _callback{callback}
{

}

lithium::RenderStage::~RenderStage() noexcept
{

}

void lithium::RenderStage::render(const glm::ivec4& defaultViewport)
{
    if(_frameBuffer)
    {
        _frameBuffer->bind();
        auto res = _frameBuffer->resolution();
        glViewport(0.0f, 0.0f, res.x, res.y);
        _callback();
        _frameBuffer->unbind();
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(defaultViewport.x, defaultViewport.y, defaultViewport.z, defaultViewport.w);
        _callback();
    }
}

glm::ivec4 lithium::RenderStage::latestViewport{0};