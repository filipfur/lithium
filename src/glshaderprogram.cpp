#include "glshaderprogram.h"

lithium::ShaderProgram* lithium::ShaderProgram::_inUse{nullptr};
unsigned int lithium::ShaderProgram::_bindCount{0};

lithium::ShaderProgram::ShaderProgram(const lithium::ShaderProgram& other) 
    : ShaderProgram{other._vertexShader, other._fragmentShader, other._geometryShader}
{
}

lithium::ShaderProgram::ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
    : ShaderProgram{
        std::shared_ptr<lithium::Shader<GL_VERTEX_SHADER>>(lithium::Shader<GL_VERTEX_SHADER>::fromFile(vertexShaderFile)),
        std::shared_ptr<lithium::Shader<GL_FRAGMENT_SHADER>>(lithium::Shader<GL_FRAGMENT_SHADER>::fromFile(fragmentShaderFile)),
        nullptr}
{
}

lithium::ShaderProgram::ShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile, const std::string& geometryShaderFile)
    : ShaderProgram{
        std::shared_ptr<lithium::Shader<GL_VERTEX_SHADER>>(lithium::Shader<GL_VERTEX_SHADER>::fromFile(vertexShaderFile)),
        std::shared_ptr<lithium::Shader<GL_FRAGMENT_SHADER>>(lithium::Shader<GL_FRAGMENT_SHADER>::fromFile(fragmentShaderFile)),
        std::shared_ptr<lithium::Shader<GL_GEOMETRY_SHADER>>(lithium::Shader<GL_GEOMETRY_SHADER>::fromFile(geometryShaderFile))}
{

}

lithium::ShaderProgram::ShaderProgram(std::shared_ptr<lithium::Shader<GL_VERTEX_SHADER>> vertexShader,
            std::shared_ptr<lithium::Shader<GL_FRAGMENT_SHADER>> fragmentShader,
            std::shared_ptr<lithium::Shader<GL_GEOMETRY_SHADER>> geometryShader)
            : _vertexShader{vertexShader}, _fragmentShader{fragmentShader}, _geometryShader{geometryShader}
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
        std::cerr << "ERROR::During link of ShaderProgram\n" << infoLog << "\n ******************************** " << std::endl;
        std::cerr << "File: " << _vertexShader->fileName() << std::endl;
        std::cerr << "File: " << _fragmentShader->fileName() << std::endl;
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
    for(auto&& pair : _vec2Cache)
    {
        setUniform(pair.first, pair.second);
    }
    for(auto&& pair : _vec3Cache)
    {
        setUniform(pair.first, pair.second);
    }
    for(auto&& pair : _vec4Cache)
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