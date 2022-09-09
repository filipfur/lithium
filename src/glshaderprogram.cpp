#include "glshaderprogram.h"

mygl::ShaderProgram* mygl::ShaderProgram::_inUse{nullptr};
unsigned int mygl::ShaderProgram::_bindCount{0};

mygl::ShaderProgram::ShaderProgram(const mygl::ShaderProgram& other) : _vertexShader{other._vertexShader}, _fragmentShader{other._fragmentShader}
{
    _id = glCreateProgram();
    glAttachShader(_id, _vertexShader.id());
    glAttachShader(_id, _fragmentShader.id());
    glLinkProgram(_id);
}

mygl::ShaderProgram::ShaderProgram(std::string &&vertexShaderFile, std::string &&fragmentShaderFile) : _vertexShader{std::move(vertexShaderFile)}, _fragmentShader{std::move(fragmentShaderFile)}
{
    _id = glCreateProgram();
    glAttachShader(_id, _vertexShader.id());
    glAttachShader(_id, _fragmentShader.id());
    glLinkProgram(_id);
}

GLuint mygl::ShaderProgram::loadUniform(const std::string &name)
{
    use();
    GLuint id = 0;
    auto it = _uniforms.find(name);
    if (it == _uniforms.end())
    {
        id = glGetUniformLocation(_id, name.c_str());
        _uniforms.emplace(name, id);
    }
    else
    {
        id = it->second;
    }
    return id;
}

void mygl::ShaderProgram::use()
{
    if(_inUse != this)
    {
        glUseProgram(_id);
        _inUse = this;
        ++_bindCount;
    }
}