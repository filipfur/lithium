#include "glcanvas.h"

lithium::Canvas::Canvas(const glm::vec2& resolution)
    : _resolution{resolution},
    _camera{glm::ortho(-resolution.x * 0.5f, resolution.x * 0.5f, -resolution.y * 0.5f, resolution.y * 0.5f, -10.0f, 10.0f)},
    //_camera{glm::ortho(0.0f, resolution.x, 0.0f, resolution.y, -10.0f, 10.0f)},
    _canvasUBO{sizeof(glm::mat4) * 2, "CanvasUBO", 0}
{
    //setPosition(glm::vec3{0.0f, 0.0f, -0.01f});
    //FrameRenderer::attach(this);
    //stage();
}

lithium::Canvas::~Canvas() noexcept
{
}

void lithium::Canvas::setFrame(std::shared_ptr<lithium::Frame> frame)
{
    _frame = frame;
    _frameMesh = _frame->mesh();
}

void lithium::Canvas::move(const glm::vec2& delta)
{
    _lookTarget += glm::vec3{delta, 0.0f};
}

void lithium::Canvas::loadUI(const std::string& path)
{
    lithium::json::Json json;
    std::ifstream ifs{path};
    ifs >> json;
    _layoutSystem.load(json);
    addFrame(_layoutSystem.root());
}

std::shared_ptr<lithium::Frame> lithium::Canvas::addFrame(lithium::FrameLayout* frameLayout)
{
    assert(frameLayout->iFrameLayout() == nullptr);
    auto frame = std::make_shared<lithium::Frame>(frameLayout);
    _frames.push_back(frame);
    if(_frame == nullptr)
    {
        assert(frameLayout->parent() == nullptr);
        setFrame(frame);
    }
    frameLayout->attach(frame.get());
    if(frameLayout->parent())
    {
        if(auto parent = dynamic_cast<lithium::Frame*>(frameLayout->parent()->iFrameLayout()))
        {
            parent->lithium::FrameRenderer::attach(frame.get());
            frame->stage();
        }
    }
    frameLayout->forEachChild([this](lithium::FrameLayout& child){
        addFrame(&child);
    });
    return frame;
}

lithium::Frame* lithium::Canvas::frameById(const std::string& id)
{
    for(auto& frame : _frames)
    {
        if(frame->layout()->id() == id)
        {
            return frame.get();
        }
    }
    return nullptr;
}

void lithium::Canvas::update(float dt)
{
    lithium::Updateable::update(dt);
    if(glm::distance2(_lookPosition, _lookTarget) < 0.0001f)
    {
        _lookPosition = _lookTarget;
    }
    else
    {
        _lookPosition = glm::mix(_lookPosition, _lookTarget, dt * 16.0f);
        //refresh();
    }
    _camera.setPosition(glm::vec3{0.0f, 0.0f, 1.0f} + _lookPosition);
    _camera.setTarget(_lookPosition);
}

/*
Note to self.
Change so that all frames are rendered by the canvas.
There is no need for a frame to be a FrameRenderer->RenderPipeline.
A frame must have a FBO/texture however.
*/

void lithium::Canvas::render()
{
    if(_frame == nullptr)
    {
        return;
    }

    _frame->renderFrames();

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

    _frame->cachedTexture()->bind(GL_TEXTURE0);
    //_downSampleStage->frameBuffer()->texture(GL_COLOR_ATTACHMENT0)->bind(GL_TEXTURE0);
    //auto frame = dynamic_cast<Frame*>(this);
    _frame->shade(lithium::Frame::shaderProgram().get());
    lithium::Frame::shaderProgram()->setUniform("u_color", glm::vec4{1.0f});
    _frameMesh->bind();
    _frameMesh->draw();
}