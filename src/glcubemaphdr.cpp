#include "glcubemaphdr.h"

#include "stb_image.h"

#include "glshaderprogram.h"
#include "glframebuffer.h"
#include "glcube.h"

namespace
{
    const char* sphericToCartesianVert = R"(#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 localPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    localPos = aPos;
    mat4 modelView = view;
    modelView[3][0] = 0.0;
    modelView[3][1] = 0.0;
    modelView[3][2] = 0.0;
    gl_Position =  projection * modelView * vec4(localPos, 1.0);
})";

    const char* sphericToCartesianFrag = R"(#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(localPos)); // make sure to normalize localPos
    vec3 color = texture(equirectangularMap, uv).rgb;

    if (isinf(color.x))
    {
        color = vec3(1000.0);
    }

    
    FragColor = vec4(color, 1.0);
})";

    const char* irradianceFrag = R"(#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359;

void main()
{		
    vec3 N = normalize(localPos);

    vec3 irradiance = vec3(0.0);   
    
    // tangent space calculation from origin point
    vec3 up    = vec3(0.0, 1.0, 0.0);
    vec3 right = normalize(cross(up, N));
    up         = normalize(cross(N, right));
       
    float sampleDelta = 0.025;
    float nrSamples = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
    {
        for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
        {
            // spherical to cartesian (in tangent space)
            vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
            // tangent space to world
            vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;

            vec3 envColor = texture(environmentMap, sampleVec).rgb;
            irradiance += envColor * cos(theta) * sin(theta);
            nrSamples++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(nrSamples));
    
    FragColor = vec4(irradiance, 1.0);
    //FragColor = vec4(texture(environmentMap, localPos).rgb, 1.0);
})";

    std::vector<GLfloat> cubeVertices = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
    };
}


lithium::CubemapHDR::CubemapHDR()
    : lithium::Texture<float>{nullptr, 0, 0, GL_FLOAT, GL_RGB16F, GL_RGB, GL_TEXTURE_CUBE_MAP}
{
}

lithium::CubemapHDR::~CubemapHDR() noexcept
{

}

std::shared_ptr<lithium::CubemapHDR> lithium::CubemapHDR::load(const std::filesystem::path& path)
{
    stbi_set_flip_vertically_on_load(true);
    int width;
    int height;
    int nrChannels;
    std::string fileName = path.string();
    float* data = stbi_loadf(fileName.c_str(), &width, &height, &nrChannels, 0);
    if(data == nullptr)
    {
        std::cout << "Failed to load HDR image." << std::endl;
        return nullptr;
    }
    static std::shared_ptr<lithium::Texture<float>> hdrTexture = std::make_shared<lithium::Texture<float>>(
        data, width, height, GL_FLOAT, GL_RGB16F, GL_RGB, GL_TEXTURE_2D);
    hdrTexture->setWrap(GL_CLAMP_TO_EDGE)->setFilter(GL_LINEAR);   
    stbi_image_free(data);
    static std::shared_ptr<lithium::ShaderProgram> sphericToCartesianShader = std::make_shared<lithium::ShaderProgram>(
        std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(sphericToCartesianVert)),
        std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(sphericToCartesianFrag)));
    static std::shared_ptr<lithium::ShaderProgram> irradianceShader = std::make_shared<lithium::ShaderProgram>(
        std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(sphericToCartesianVert)),
        std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(irradianceFrag)));
    int faceWidth{1024};
    int faceHeight{1024};
    int irradianceSize{64};
    static lithium::FrameBuffer captureFBO{glm::ivec2{faceWidth, faceHeight}};
    captureFBO.bind();
    captureFBO.createRenderBuffer(GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT, false);
    captureFBO.unbind();
    static auto cubemap = std::shared_ptr<lithium::CubemapHDR>(new lithium::CubemapHDR());
    cubemap->_irradianceMap.reset(
        new Texture<float>(nullptr, 0, 0, GL_FLOAT, GL_RGB16F, GL_RGB, GL_TEXTURE_CUBE_MAP)
    );
    const auto initCubemap = [](auto cubemap, float faceWidth, float faceHeight)
    {
        cubemap->bind();
        for(int i = 0; i < 6; i++)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F,
                faceWidth, faceHeight, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        cubemap->setFilter(GL_LINEAR);
        cubemap->setWrap(GL_CLAMP_TO_EDGE);
    };
    initCubemap(cubemap, faceWidth, faceHeight);
    initCubemap(cubemap->_irradianceMap, irradianceSize, irradianceSize);

    // render cubemap textures

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] = 
    {
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    // convert HDR equirectangular environment map to cubemap equivalent
    sphericToCartesianShader->setUniform("equirectangularMap", 0);
    sphericToCartesianShader->setUniform("projection", captureProjection);
    hdrTexture->bind(GL_TEXTURE0);

    /*static std::shared_ptr<lithium::Mesh> cube = std::make_shared<lithium::Mesh>(
        std::vector<lithium::VertexArrayBuffer::AttributeType>
        {lithium::VertexArrayBuffer::AttributeType::VEC3,
        lithium::VertexArrayBuffer::AttributeType::VEC3,
        lithium::VertexArrayBuffer::AttributeType::VEC2},
        cubeVertices
    );*/
    static std::shared_ptr<lithium::Mesh> cube;
    cube.reset(lithium::Cube(glm::vec3{1.0f}));

    glViewport(0, 0, faceWidth, faceHeight); // don't forget to configure the viewport to the capture dimensions.
    captureFBO.bind();
    glDisable(GL_CULL_FACE);
    for (unsigned int i = 0; i < 6; ++i)
    {
        sphericToCartesianShader->setUniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap->id(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube->bind();
        cube->draw();
    }

    captureFBO.bind();
    captureFBO.renderBuffer()->bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradianceSize, irradianceSize); // scale down to 32x32

    irradianceShader->setUniform("environmentMap", 0);
    irradianceShader->setUniform("projection", captureProjection);
    cubemap->bind(GL_TEXTURE0);

    glViewport(0, 0, irradianceSize, irradianceSize); // don't forget to configure the viewport to the capture dimensions.
    captureFBO.bind();
    for (unsigned int i = 0; i < 6; ++i)
    {
        irradianceShader->setUniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap->_irradianceMap->id(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube->bind();
        cube->draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
    glEnable(GL_CULL_FACE);

    return cubemap;
}

void lithium::CubemapHDR::draw(lithium::CubemapHDR& cubemapHDR, const lithium::SimpleCamera& camera)
{
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static glm::mat4 captureViews[] = 
    {
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };
    static size_t currentView{0};
    static int cntr{0};
    static std::shared_ptr<lithium::ShaderProgram> irradianceShader = std::make_shared<lithium::ShaderProgram>(
        std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(sphericToCartesianVert)),
        std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(irradianceFrag)));
    if(cntr++ % 100 == 0)
    {
        irradianceShader->setUniform("environmentMap", 0);
        irradianceShader->setUniform("projection", glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f));
        currentView = (currentView + 1) % 6;
        irradianceShader->setUniform("view", captureViews[currentView]);
        std::cout << "TIMER" << std::endl;
    }

    static std::shared_ptr<lithium::Mesh> cube = std::shared_ptr<lithium::Mesh>(lithium::Cube());
    cubemapHDR.bind(GL_TEXTURE0);
    cube->bind();
    cube->draw();
    glEnable(GL_CULL_FACE);
}