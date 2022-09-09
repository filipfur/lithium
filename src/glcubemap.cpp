#include "glcubemap.h"

#include "stb_image.h"
#include <sstream>

mygl::Cubemap::Cubemap(mygl::ShaderProgram* shaderProgram, mygl::Mesh* mesh, const std::string& fileName, const std::string& fileExt) : _shaderProgram{shaderProgram}, _mesh{mesh}
{
    glGenTextures(1, &_id);
    bind();
    int width, height, nrChannels;
    unsigned char *data;
    std::vector<std::string> faces{
		"right." + fileExt,
		"left." + fileExt,
		"top." + fileExt,
		"bottom." + fileExt,
		"front." + fileExt,
		"back." + fileExt
	};
    stbi_set_flip_vertically_on_load(false);
    for(unsigned int i = 0; i < 6; i++)
    {
        std::stringstream ss{};
        ss << fileName << "/" << faces[i];
        data = stbi_load(ss.str().c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    unbind();
}

mygl::Cubemap::~Cubemap()
{

}

void mygl::Cubemap::draw(Camera* camera)
{
    glDepthFunc(GL_LEQUAL);
    _shaderProgram->use();
    // ... set view and projection matrix
    //glBindVertexArray(skyboxVAO);
    glUniformMatrix4fv(glGetUniformLocation(_shaderProgram->id(), "u_camera"), 1, GL_FALSE, glm::value_ptr(camera->rotationMatrix()));

    _mesh->bindVertexArray();
    glActiveTexture(GL_TEXTURE0);
    bind();
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    _mesh->drawElements();
    glDepthFunc(GL_LESS);
    unbind();
}

void mygl::Cubemap::bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
}

void mygl::Cubemap::unbind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}