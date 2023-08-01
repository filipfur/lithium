#include "glframe.h"

#include "glplane.h"

namespace
{
    const char* vertexSrc = R"(#version 330 core
        layout(location = 0) in vec2 aPos;
        layout(location = 1) in vec2 aTexCoords;

        layout (std140) uniform CanvasUBO
        {
            mat4 u_projection;
            mat4 u_view;
        };

        out vec2 TexCoords;

        uniform mat4 u_model;

        void main()
        {
            TexCoords = aTexCoords;
            gl_Position = u_projection * u_view * u_model * vec4(aPos, 0.0, 1.0);
        }
    )";
    
    const char* fragmentSrc = R"(#version 330 core
        out vec4 fragColor;

        uniform sampler2D u_texture_0;
        uniform vec4 u_color;

        in vec2 TexCoords;

        void main()
        {
            vec4 texColor = texture(u_texture_0, TexCoords);
            if(texColor.a < 0.01)
            {
                discard;
            }
            fragColor = texColor * u_color;
        }
    )";
}

std::shared_ptr<lithium::Mesh> mySharedMesh() {
    static const std::shared_ptr<lithium::Mesh> mesh{lithium::Plane2D(glm::vec2{0.5f})};
    return mesh;
};

lithium::Object::TexturePointer mySharedTexture() {
    static const lithium::Object::TexturePointer texture = lithium::Texture<unsigned char>::Basic();
    texture->setFilter(GL_NEAREST);
    return texture;
};

lithium::Frame::Frame(FrameLayout* frameLayout) : Object{mySharedMesh(), {mySharedTexture()}},  FrameRenderer{frameLayout->actualDimension()}, _frameLayout{frameLayout}//,
    /*_parent{frameLayout->_parent ? dynamic_cast<lithium::Frame*>(frameLayout->_parent->_iFrameLayout) : nullptr}*/
{
    setPosition(glm::vec3{frameLayout->actualPosition(), 0.0f});
    setScale(glm::vec3{frameLayout->actualDimension(), 1.0f});
    setObjectName(frameLayout->id());
    _frameLayout->attach(this);
}

lithium::Frame::~Frame() noexcept
{
}

std::shared_ptr<lithium::ShaderProgram> lithium::Frame::shaderProgram()
{
    static const std::shared_ptr<lithium::ShaderProgram> shaderProgram = std::make_shared<lithium::ShaderProgram>(
        std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(vertexSrc)),
        std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(fragmentSrc)));
    return shaderProgram;
}

bool lithium::Frame::renderFrames()
{
    bool childRendered{false};
    forEachChild([&childRendered](Frame* frame)
    {
        if(frame->hasChildren() || frame->textRenderer())
        {
            childRendered |= frame->renderFrames();
        }
    });
    bool reRender = _changed || childRendered;
    //_renderStages[0]->setEnabled(reRender);
    //_renderStages[1]->setEnabled(reRender);
    if(reRender)
    {
        FrameRenderer::render();
    }
    //glFinish();
    _changed = false;
    return reRender;
}