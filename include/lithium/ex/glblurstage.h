#pragma once

#include "glrenderstage.h"
#include "gltexture.h"
#include "glplane.h"

namespace
{
    const char* vertexSrcBlur = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoord;

void main()
{
    texCoord = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
})";

    const char* fragmentSrcBlur = R"(#version 330 core
out vec4 fragColor;

uniform sampler2D u_texture;

in vec2 texCoord;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(u_texture, 0));
    vec4 result = vec4(0.0);
    for(int x=-2; x < 3; ++x)
    {
        for(int y=-2; y < 3; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(u_texture, texCoord + offset);
        }
    }
    fragColor = result / 25.0;
})";
}


namespace lithium
{
    class ExBlurStage : public RenderStage
    {
    public:
        ExBlurStage(std::shared_ptr<lithium::FrameBuffer> frameBuffer,
            std::shared_ptr<lithium::Texture<unsigned char>> inputTexture)
            : RenderStage{frameBuffer, [this](){ 
                    static std::shared_ptr<lithium::Mesh> screen = std::shared_ptr<lithium::Mesh>(lithium::Plane2D());
                    static std::shared_ptr<lithium::ShaderProgram> blurShader = std::make_shared<lithium::ShaderProgram>(
                        std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(vertexSrcBlur)),
                        std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(fragmentSrcBlur))
                    );
                    glClear(GL_COLOR_BUFFER_BIT);
                    blurShader->use();
                    _inputTexture->bind(GL_TEXTURE0);
                    screen->bind();
                    screen->draw();
             }}, _resolution{inputTexture->width(), inputTexture->height()}, _inputTexture{inputTexture}
        {
            
        }

        virtual ~ExBlurStage() noexcept
        {

        }

        std::shared_ptr<lithium::Texture<unsigned char>> outputTexture()
        {
            return frameBuffer()->texture(GL_COLOR_ATTACHMENT0);
        }

        void setInputTexture(std::shared_ptr<lithium::Texture<unsigned char>> inputTexture)
        {
            _inputTexture = inputTexture;
        }

    private:
        const glm::ivec2 _resolution;
        bool _first{true};
        std::shared_ptr<lithium::Texture<unsigned char>> _inputTexture{nullptr};
        std::shared_ptr<lithium::ShaderProgram> _blurShader{nullptr};
    };
}