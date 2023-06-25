#include "glframerenderer.h"

#include "glframe.h"
#include "glplane.h"

lithium::FrameRenderer::FrameRenderer(const glm::vec2& dimension)
    : RenderPipeline{glm::ivec2{dimension}}, _dimension{dimension},
    _camera{glm::ortho(-dimension.x * 0.5f, dimension.x * 0.5f, -dimension.y * 0.5f, dimension.y * 0.5f, -10.0f, 10.0f)},
    //_camera{glm::ortho(0.0f, dimension.x, 0.0f, dimension.y, -10.0f, 10.0f)},
    _msaaFBO{std::make_shared<lithium::FrameBuffer>(dimension)},
    _textureFBO{std::make_shared<lithium::FrameBuffer>(dimension)},
    _canvasUBO{sizeof(glm::mat4) * 2, "CanvasUBO", 0}
{
    static const auto msaaShader = std::make_shared<lithium::ShaderProgram>("shaders/screenshader.vert", "shaders/msaa.frag");
    _msaaShader = msaaShader;
    _msaaShader->setUniform("u_texture", 0);

    _camera.setPosition(glm::vec3{0.0f, 0.0f, 1.0f});
    _camera.setTarget(glm::vec3{0.0f, 0.0f, 0.0f});
    _frameShader = lithium::Frame::shaderProgram();

    _canvasUBO.bindBufferBase({
        _frameShader.get()
    });
    _canvasUBO.bufferSubData(0, _camera.projection());

    _frameRenderGroup = createRenderGroup([this](lithium::Renderable* renderable) -> bool {
        return !renderable->hasAttachments(); // All renderables without attachments are frames.
    });

    static const auto screen = std::shared_ptr<lithium::Mesh>(lithium::Plane3D());

    /*_msaaFBO->bind();
    _msaaFBO->createTexture(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT, GL_TEXTURE_2D_MULTISAMPLE);
    //_msaaFBO->createTexture(GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH24_STENCIL8, GL_DEPTH24_STENCIL8, GL_FLOAT, GL_TEXTURE_2D_MULTISAMPLE);
    //_msaaFBO->declareBuffers();
    _msaaFBO->unbind();*/

    _textureFBO->bind();
    _textureFBO->createTexture(GL_COLOR_ATTACHMENT0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_TEXTURE_2D);//->setWrap(GL_REPEAT);
    _textureFBO->createRenderBuffer();
    _textureFBO->declareBuffers();
    //_textureFBO->createTexture(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT);
    _textureFBO->unbind();


    /*_mainStage = addRenderStage(std::make_shared<lithium::RenderStage>(_msaaFBO, glm::ivec4{0.0f, 0.0f, dimension.x * 2.0f, dimension.y  * 2.0f}, [this](){
        clearColor(0.0f, 0.0f, 0.0f, 1.0f);
        clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        disableDepthTesting();

        _canvasUBO.bufferSubData(0, _camera.projection());
        _canvasUBO.bufferSubData(sizeof(glm::mat4), glm::mat4{1.0f});
        _canvasUBO.bindBufferBase({
            _frameShader.get()
        });
        auto frame = dynamic_cast<Frame*>(this);
        frame->shade(lithium::Frame::shaderProgram().get());
        frame->mesh()->bind();
        frame->mesh()->draw();
        _frameRenderGroup->render(_frameShader);
        _textRenderer.render();
        enableDepthTesting();
        //std::cout << "Re-rendered " << _frameRenderGroup->count() << " frames." << std::endl;
    }));

    _downSampleStage = addRenderStage(std::make_shared<lithium::RenderStage>(_textureFBO, glm::ivec4{0.0f, 0.0f, dimension.x * 2.0f, dimension.y  * 2.0f}, [this](){
        clearColor(0.0f, 0.0f, 0.0f, 1.0f);
        clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        disableDepthTesting();
        _msaaShader->use();
        _mainStage->frameBuffer()->texture(GL_COLOR_ATTACHMENT0)->bind(GL_TEXTURE0);
        screen->bind();
        screen->draw();
        enableDepthTesting();
    }));*/

    _mainStage = addRenderStage(std::make_shared<lithium::RenderStage>(_textureFBO, [this](){
        clearColor(0.0f, 0.0f, 0.0f, 0.0f);
        clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //disableDepthTesting();

        disableDepthTesting();
        //disableBlending();
        enableBlending();

        _canvasUBO.bufferSubData(0, _camera.projection());
        _canvasUBO.bufferSubData(sizeof(glm::mat4), glm::mat4{1.0f});
        _canvasUBO.bindBufferBase({
            _frameShader.get()
        });
        auto frame = dynamic_cast<Frame*>(this);
        /*frame->shade(lithium::Frame::shaderProgram().get());
        frame->mesh()->bind();
        frame->mesh()->draw();*/
        auto p0 = frame->position();
        frame->setPosition(glm::vec3{0.0f});
        frame->lithium::Renderable::render(_frameShader.get());
        std::cout << "Rendering: " << frame->objectName() << std::endl;
        if(frame->_textRenderer)
        {
            frame->_textRenderer->render();
        }
        //_frameRenderGroup->render(_frameShader);
        //clear(GL_DEPTH_BUFFER_BIT);
        _frameRenderGroup->forEach([this](lithium::Renderable* renderable){
            auto fr = dynamic_cast<Frame*>(renderable);
            fr->shade(_frameShader.get());
            if(fr->hasChildren() || fr->textRenderer())
            {
                _frameShader->setUniform("u_color", glm::vec4{1.0f});
                fr->cachedTexture()->bind(GL_TEXTURE0);
                fr->mesh()->bind(); // TODO: Will cause problem if this has messed with the UVs e.g.
                fr->mesh()->draw();
            }
            else
            {
                fr->draw();
            }
            std::cout << "  Rendering: " << fr->objectName() << std::endl;
        });
        frame->setPosition(p0);
        //enableDepthTesting();
        //enableBlending();
        //std::cout << "Re-rendered " << _frameRenderGroup->count() << " frames." << std::endl;
    }));
}

lithium::FrameRenderer::~FrameRenderer() noexcept
{

}