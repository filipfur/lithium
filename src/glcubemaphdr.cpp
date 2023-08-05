#include "glcubemaphdr.h"

#include "stb_image.h"

#include "glshaderprogram.h"
#include "glframebuffer.h"
#include "glcube.h"
#include "glplane.h"

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

    const char* prefilterFrag = R"(#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform samplerCube environmentMap;
uniform float roughness;

const float PI = 3.14159265359;


float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}
  
void main()
{		
    vec3 N = normalize(localPos);    
    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    float totalWeight = 0.0;   
    vec3 prefilteredColor = vec3(0.0);     
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0)
        {
            prefilteredColor += texture(environmentMap, L).rgb * NdotL;
            totalWeight      += NdotL;
        }
    }
    prefilteredColor = prefilteredColor / totalWeight;

    FragColor = vec4(prefilteredColor, 1.0);
})";


    const char* brdfVert = R"(#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 0.0, 1.0);
})";

    const char* brdfFrag = R"(#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

#include <lightning.glsl>

float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
    return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
    float a = roughness*roughness;
	
    float phi = 2.0 * PI * Xi.x;
    float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
    float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
    // from spherical coordinates to cartesian coordinates
    vec3 H;
    H.x = cos(phi) * sinTheta;
    H.y = sin(phi) * sinTheta;
    H.z = cosTheta;
	
    // from tangent-space vector to world-space sample vector
    vec3 up        = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent   = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);
	
    vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sampleVec);
}

vec2 IntegrateBRDF(float NdotV, float roughness)
{
    vec3 V;
    V.x = sqrt(1.0 - NdotV*NdotV);
    V.y = 0.0;
    V.z = NdotV;

    float A = 0.0;
    float B = 0.0;

    vec3 N = vec3(0.0, 0.0, 1.0);

    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if(NdotL > 0.0)
        {
            float G = GeometrySmith(N, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    return vec2(A, B);
}
// ----------------------------------------------------------------------------
void main() 
{
    vec2 integratedBRDF = IntegrateBRDF(TexCoords.x, TexCoords.y);
    FragColor = vec4(integratedBRDF, 0.0, 1.0);
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
    static std::shared_ptr<lithium::ShaderProgram> prefilterShader = std::make_shared<lithium::ShaderProgram>(
        std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(sphericToCartesianVert)),
        std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(prefilterFrag)));
    static std::shared_ptr<lithium::ShaderProgram> brdfShader = std::make_shared<lithium::ShaderProgram>(
        std::shared_ptr<lithium::VertexShader>(lithium::VertexShader::fromSource(brdfVert)),
        std::shared_ptr<lithium::FragmentShader>(lithium::FragmentShader::fromSource(
            lithium::FragmentShader::expandSource("", brdfFrag))));
    const int faceWidth{2048};
    const int faceHeight{2048};
    const int irradianceSize{64};
    const int prefilterSize{256};
    const int brdfLUTSize{512};
    static lithium::FrameBuffer captureFBO{glm::ivec2{faceWidth, faceHeight}};
    captureFBO.bind();
    captureFBO.createRenderBuffer(GL_DEPTH_COMPONENT24, GL_DEPTH_ATTACHMENT, false);
    captureFBO.unbind();
    static auto cubemap = std::shared_ptr<lithium::CubemapHDR>(new lithium::CubemapHDR());
    cubemap->_irradianceMap.reset(
        new Texture<float>(nullptr, 0, 0, GL_FLOAT, GL_RGB16F, GL_RGB, GL_TEXTURE_CUBE_MAP)
    );
    cubemap->_prefilterMap.reset(
        new Texture<float>(nullptr, 0, 0, GL_FLOAT, GL_RGB16F, GL_RGB, GL_TEXTURE_CUBE_MAP)
    );
    cubemap->_brdfLUT.reset(
        new Texture<float>(nullptr, 0, 0, GL_FLOAT, GL_RG16F, GL_RG, GL_TEXTURE_2D)
    );
    cubemap->_brdfLUT->bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, brdfLUTSize, brdfLUTSize, 0, GL_RG, GL_FLOAT, nullptr);
    cubemap->_brdfLUT->setFilter(GL_LINEAR);
    cubemap->_brdfLUT->setWrap(GL_CLAMP_TO_EDGE);

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
    
    initCubemap(cubemap->_prefilterMap, prefilterSize, prefilterSize);
    cubemap->_prefilterMap->setFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    cubemap->_prefilterMap->bind();
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    // render cubemap textures
    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

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

    auto renderFaces = [](std::shared_ptr<lithium::ShaderProgram> shader, std::shared_ptr<Texture<float>> cubemap, int mip=0) {
        static glm::mat4 captureViews[] = {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };
        static std::shared_ptr<lithium::Mesh> cube = std::shared_ptr<lithium::Mesh>(lithium::Cube(glm::vec3{1.0f}));
        for (unsigned int i = 0; i < 6; ++i)
        {
            shader->setUniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 
                                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubemap->id(), mip);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            cube->bind();
            cube->draw();
        }
    };

    glViewport(0, 0, faceWidth, faceHeight); // don't forget to configure the viewport to the capture dimensions.
    captureFBO.bind();
    glDisable(GL_CULL_FACE);
    renderFaces(sphericToCartesianShader, cubemap);

    captureFBO.bind();
    captureFBO.renderBuffer()->bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradianceSize, irradianceSize); // scale down to 32x32
    irradianceShader->setUniform("environmentMap", 0);
    irradianceShader->setUniform("projection", captureProjection);
    cubemap->bind(GL_TEXTURE0);
    glViewport(0, 0, irradianceSize, irradianceSize); // don't forget to configure the viewport to the capture dimensions.
    captureFBO.bind();
    renderFaces(irradianceShader, cubemap->_irradianceMap);
    
    prefilterShader->setUniform("environmentMap", 0);
    prefilterShader->setUniform("projection", captureProjection);
    cubemap->bind(GL_TEXTURE0);
    captureFBO.bind();
    int maxMipLevels = static_cast<int>(glm::log2(static_cast<float>(prefilterSize))) - 3;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
    {
        // reisze framebuffer according to mip-level size.
        unsigned int mipWidth  = prefilterSize * std::pow(0.5, mip);
        unsigned int mipHeight = prefilterSize * std::pow(0.5, mip);
        captureFBO.renderBuffer()->bind();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
        glViewport(0, 0, mipWidth, mipHeight);
        float roughness = (float)mip / (float)(maxMipLevels - 1);
        prefilterShader->setUniform("roughness", roughness);
        captureFBO.bind();
        renderFaces(prefilterShader, cubemap->_prefilterMap, mip);
    }

    captureFBO.bind();
    captureFBO.renderBuffer()->bind();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, brdfLUTSize, brdfLUTSize);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cubemap->_brdfLUT->id(), 0);


    static std::shared_ptr<lithium::Mesh> quad = std::shared_ptr<lithium::Mesh>(lithium::Plane2D());

    glViewport(0, 0, brdfLUTSize, brdfLUTSize);
    brdfShader->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    quad->bind();
    quad->draw();
    
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