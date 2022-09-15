#include "glshaderprogram.h"

lithium::ShaderProgram* lithium::ShaderProgram::_inUse{nullptr};
unsigned int lithium::ShaderProgram::_bindCount{0};

lithium::ShaderProgram::ShaderProgram(const lithium::ShaderProgram& other) : _vertexShader{other._vertexShader}, _fragmentShader{other._fragmentShader}
{
    _id = glCreateProgram();
    link();
}

lithium::ShaderProgram::ShaderProgram(std::string &&vertexShaderFile, std::string &&fragmentShaderFile) : _vertexShader{std::move(vertexShaderFile)}, _fragmentShader{std::move(fragmentShaderFile)}
{
    _id = glCreateProgram();
    link();
}

GLuint lithium::ShaderProgram::loadUniform(const std::string &name)
{
    use();
    GLuint id = 0;
    auto it = _uniforms.find(name);
    if (it == _uniforms.end())
    {
        id = glGetUniformLocation(_id, name.c_str());
        if(id == INVALID_LOCATION)
        {
            std::cerr << "Failed to load uniform: " << name << std::endl;
        }
        _uniforms.emplace(name, id);
    }
    else
    {
        id = it->second;
    }
    return id;
}

void lithium::ShaderProgram::use()
{
    bool compiledShader{false};
    if(!_fragmentShader.valid())
    {
        _fragmentShader.compile();
        compiledShader = true;
    }
    if(!_vertexShader.valid())
    {
        _vertexShader.compile();
        compiledShader = true;
    }
    if(compiledShader)
    {
        link();
    }

    if(_inUse != this)
    {
        glUseProgram(_id);
        _inUse = this;
        ++_bindCount;
    }
}