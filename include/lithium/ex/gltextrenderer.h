#pragma once

#include <set>
#include "gltext.h"
#include "glshaderprogram.h"
#include "glorthographiccamera.h"
#include "glrendergroup.h"

namespace
{
    const char* vertexSrcSdfText = R"(#version 330 core

    layout (location = 0) in vec4 aVertex;

    uniform mat4 u_projection;
    uniform mat4 u_view;
    uniform mat4 u_model;

    out vec2 texCoord;
    out vec2 position;

    void main() {
    texCoord = aVertex.zw;
    position = vec2(aVertex.x, -aVertex.y);
    vec3 p0 = vec3(u_model * vec4(position, 0.0, 1.0));
    gl_Position = u_projection * u_view * vec4(p0, 1.0);
    }
    )";

    const char* fragmentSrcSdfText = R"( #version 330 core
    //precision mediump float;

    out vec4 FragColor;

    uniform vec4 u_color;
    uniform sampler2D u_texture;

    in vec2 texCoord;
    in vec2 position;

    void main() {
        float r = texture(u_texture, texCoord).r;
        float scale = 1.0 / fwidth(r);
        float signedDistance = (r - 0.5) * scale;
        float borderWidth = 0.2;//0.125;
        float color = clamp(signedDistance + 0.5, 0.0, 1.0);
        float alpha = clamp(signedDistance + 0.5 + scale * borderWidth, 0.0, 1.0);
        FragColor = vec4(u_color.rgb * color, u_color.a) * alpha;
        //FragColor += vec4(1, 0, 0, 1) * 0.5;
    }
    )";
}

namespace lithium
{
    class ExTextRenderer : public Updateable
    {        
    public:
        ExTextRenderer(const glm::vec2& resolution)
            : _resoultion{resolution},
            _camera{glm::ortho(-resolution.x * 0.5f, resolution.x * 0.5f, -resolution.y * 0.5f, resolution.y * 0.5f, -10.0f, 10.0f)}
        {
            _textShader = std::make_shared<lithium::ShaderProgram>(
                std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(vertexSrcSdfText)),
                std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(fragmentSrcSdfText))
            );
            _camera.setPosition(glm::vec3{0.0f, 0.0f, 1.0f});
            _camera.setTarget(glm::vec3{0.0f, 0.0f, 0.0f});
            _textShader->setUniform("u_projection", _camera.projection());
            _textShader->setUniform("u_view", _camera.view());
        }

        virtual ~ExTextRenderer() noexcept
        {

        }

        void render(std::shared_ptr<RenderGroup> renderGroup)
        {
            renderGroup->render(_textShader.get());
        }

        void render(std::shared_ptr<lithium::Text> text)
        {
            text->render(_textShader.get());
        }

        void update(float dt)
        {
            //Updateable::update(dt);
            for(auto& text : _texts)
            {
                text->update(dt);
            }
        }

        void render()
        {
            for(auto& text : _texts)
            {
                text->render(_textShader.get());
            }
        }

        std::shared_ptr<lithium::Text> createText(std::shared_ptr<Font> font, const std::string& str, float textScale=1.0f)
        {
            std::shared_ptr<lithium::Text> text = std::make_shared<lithium::Text>(font, str, textScale);
            _texts.insert(text);
            return text;
        }

    private:
        glm::vec2 _resoultion;
        lithium::SimpleCamera _camera;
        std::shared_ptr<lithium::ShaderProgram> _textShader;

        std::set<std::shared_ptr<lithium::Text>> _texts;
    };
}