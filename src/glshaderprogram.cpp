#include "glshaderprogram.h"

lithium::ShaderProgram* lithium::ShaderProgram::_inUse{nullptr};
unsigned int lithium::ShaderProgram::_bindCount{0};

lithium::ShaderProgram::ShaderProgram(const lithium::ShaderProgram& other) : _vertexShader{other._vertexShader}, _fragmentShader{other._fragmentShader}
{
    _id = glCreateProgram();
    link();
}

lithium::ShaderProgram::ShaderProgram(std::string &&vertexShaderFile, std::string &&fragmentShaderFile)
    : _vertexShader{new Shader<GL_VERTEX_SHADER>(std::move(vertexShaderFile))}, _fragmentShader{new Shader<GL_FRAGMENT_SHADER>(std::move(fragmentShaderFile))}
{
    _id = glCreateProgram();
    link();
}

lithium::ShaderProgram::ShaderProgram(std::string &&vertexShaderFile, std::string &&fragmentShaderFile, std::string &&geometryShaderFile)
    : _vertexShader{new Shader<GL_VERTEX_SHADER>(std::move(vertexShaderFile))}, _fragmentShader{new Shader<GL_FRAGMENT_SHADER>(std::move(fragmentShaderFile))},
    _geometryShader{new Shader<GL_GEOMETRY_SHADER>(std::move(geometryShaderFile))}
{
    _id = glCreateProgram();
    link();
}

void lithium::ShaderProgram::link()
{
    glAttachShader(_id, _vertexShader->id());
    glAttachShader(_id, _fragmentShader->id());
    if(_geometryShader)
    {
        glAttachShader(_id, _geometryShader->id());
    }
    glLinkProgram(_id);
    GLint result = GL_FALSE;
    glGetProgramiv(_id, GL_LINK_STATUS, &result);
    int info_log_length = 1024;
    char * infoLog =  new char[info_log_length];
    if (!result)
    {
        glGetProgramInfoLog(_id, info_log_length, NULL, infoLog);
        std::cout << "ERROR::During link of ShaderProgram\n" << infoLog << "\n ******************************** " << std::endl;
        exit(1);
    }
    for(auto&& pair : _intCache)
    {
        setUniform(pair.first, pair.second);
    }
    for(auto&& pair : _mat4Cache)
    {
        setUniform(pair.first, pair.second);
    }
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
            //std::cerr << "Failed to load uniform '" << name << "' inside " << _fragmentShader->fileName() << std::endl;
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
    if(!_fragmentShader->valid())
    {
        _fragmentShader->compile();
        compiledShader = true;
    }
    if(!_vertexShader->valid())
    {
        _vertexShader->compile();
        compiledShader = true;
    }
    if(_geometryShader && !_geometryShader->valid())
    {
        _geometryShader->compile();
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