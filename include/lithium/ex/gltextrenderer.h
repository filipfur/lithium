#pragma once

#include "glshaderprogram.h"
#include "glorthographiccamera.h"

namespace lithium
{
    class ExTextRenderer
    {        
    public:
        ExTextRenderer(const glm::vec4& viewport)
            : _orthoCamera{viewport[0], viewport[2], viewport[1], viewport[3], -10000.0f, 10000.0f},
            _textShader{std::make_shared<lithium::ShaderProgram>(
                std::shared_ptr<lithium::Shader<GL_VERTEX_SHADER>>(
                    lithium::Shader<GL_VERTEX_SHADER>::fromSource(vertexSource)),
                std::shared_ptr<lithium::Shader<GL_FRAGMENT_SHADER>>(
                    lithium::Shader<GL_FRAGMENT_SHADER>::fromSource(fragmentSource))
            )}
        {
            _textShader->setUniform("u_camera", _orthoCamera.matrix());
        }

        virtual ~ExTextRenderer() noexcept
        {

        }

        void render(std::shared_ptr<RenderGroup> renderGroup)
        {
            renderGroup->render(_textShader.get());
        }

    private:
        lithium::OrthographicCamera _orthoCamera;
        std::shared_ptr<lithium::ShaderProgram> _textShader;

        inline static const std::string vertexSource{
"#version 460 core\n"
"layout (location = 0) in vec4 aVertex;\n"
"uniform mat4 u_camera;\n"
"uniform mat4 u_model;\n"
"out vec2 texCoord;\n"
"out vec2 position;\n"
"void main() {\n"
"   texCoord = aVertex.zw;\n"
"   position = vec2(aVertex.x, -aVertex.y);\n"
"   vec3 p0 = vec3(u_model * vec4(position, 0.0, 1.0));\n"
"   gl_Position = u_camera * vec4(p0, 1.0);\n"
"}\n"
        };

        inline static const std::string fragmentSource{
"#version 460 core\n"
"out vec4 FragColor;\n"
"uniform vec4 u_color;\n"
"uniform sampler2D u_texture;\n"
"in vec2 texCoord;\n"
"in vec2 position;\n"
"void main() {\n"
"    float sample = texture(u_texture, texCoord).r;\n"
"    float scale = 1.0 / fwidth(sample);\n"
"    float signedDistance = (sample - 0.5) * scale;\n"
"    float borderWidth = 0.2;//0.125;\n"
"    float color = clamp(signedDistance + 0.5, 0.0, 1.0);\n"
"    float alpha = clamp(signedDistance + 0.5 + scale * borderWidth, 0.0, 1.0);\n"
"    FragColor = vec4(u_color.rgb * color, u_color.a) * alpha;\n"
" }\n"
        };
    };
}