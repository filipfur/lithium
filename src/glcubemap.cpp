#include "glcubemap.h"

#include "stb_image.h"

lithium::Cubemap::Cubemap()
    : lithium::Texture<unsigned char>{nullptr, 0, 0, GL_UNSIGNED_BYTE, GL_RGB, GL_RGB, GL_TEXTURE_CUBE_MAP}
{
}

std::shared_ptr<lithium::Cubemap> lithium::Cubemap::load(const std::vector<std::string>& faces)
{
    auto cubemap = std::shared_ptr<lithium::Cubemap>(new lithium::Cubemap());
    cubemap->bind();
    stbi_set_flip_vertically_on_load(false);
    for(int i = 0; i < faces.size(); i++)
    {
        int width, height, nrChannels;
        unsigned char* buffer = stbi_load(faces.at(i).c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer
        );
        stbi_image_free(buffer);
    }
    cubemap->setFilter(GL_LINEAR);
    cubemap->setWrap(GL_CLAMP_TO_EDGE);
    cubemap->unbind();
    return cubemap;
}

lithium::Cubemap::~Cubemap() noexcept
{

}