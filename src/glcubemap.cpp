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

/*
void lithium::Cubemap::draw(Camera* camera)
{
    glDepthFunc(GL_LEQUAL);
    _shaderProgram->use();
    // ... set view and projection matrix
    //glBindVertexArray(skyboxVAO);
    
    //    TODO: See this next line has been commented out and its not good.


    //glUniformMatrix4fv(glGetUniformLocation(_shaderProgram->id(), "u_camera"), 1, GL_FALSE, glm::value_ptr(camera->rotationMatrix()));

    _mesh->bind();
    glActiveTexture(GL_TEXTURE0);
    bind();
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    _mesh->draw();
    glDepthFunc(GL_LESS);
    unbind();
}
*/