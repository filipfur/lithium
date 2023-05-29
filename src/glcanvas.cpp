#include "glcanvas.h"

lithium::Canvas::Canvas(const glm::vec2& resolution, const glm::vec2& dimension)
    : Frame{nullptr, dimension},
    _resolution{resolution},
    _camera{glm::ortho(-resolution.x * 0.5f, resolution.x * 0.5f, -resolution.y * 0.5f, resolution.y * 0.5f, -10.0f, 10.0f)},
    //_camera{glm::ortho(0.0f, resolution.x, 0.0f, resolution.y, -10.0f, 10.0f)},
    _canvasUBO{sizeof(glm::mat4) * 2, "CanvasUBO", 0}
{
    setPosition(glm::vec3{0.0f, 0.0f, -0.01f});
    //FrameRenderer::attach(this);
    //stage();
    _frameMesh = mesh();
}

lithium::Canvas::~Canvas() noexcept
{
}

void lithium::Canvas::renderCanvas()
{
    renderFrames();

    glViewport(0, 0, _resolution.x, _resolution.y);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _canvasUBO.bufferSubData(0, _camera.projection());
    _canvasUBO.bufferSubData(sizeof(glm::mat4), _camera.view());
    _canvasUBO.bindBufferBase({
        lithium::Frame::shaderProgram().get()
    });


    /*forEachChild([this](Frame* frame) {
        if(frame->hasChildren()) {
            frame->cachedTexture()->bind(GL_TEXTURE0);
            frame->shade(lithium::Frame::shaderProgram().get());
            lithium::Frame::shaderProgram()->setUniform("u_color", glm::vec4{1.0f});
            frame->mesh()->bind();
            frame->mesh()->draw();
        }
    });
    return; // !!!*/

    cachedTexture()->bind(GL_TEXTURE0);
    //_downSampleStage->frameBuffer()->texture(GL_COLOR_ATTACHMENT0)->bind(GL_TEXTURE0);
    //auto frame = dynamic_cast<Frame*>(this);
    shade(lithium::Frame::shaderProgram().get());
    lithium::Frame::shaderProgram()->setUniform("u_color", glm::vec4{1.0f});
    _frameMesh->bind();
    _frameMesh->draw();
}