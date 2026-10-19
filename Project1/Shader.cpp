#include "Shader.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

Shader::Shader(GLenum shaderType, const char* shaderFile)
    : id(0)
{
    std::ifstream file(shaderFile);

    if (!file.is_open())
    {
        std::cout << "Failed to open shader file: "
            << shaderFile << std::endl;
        return;
    }

    std::string source(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    const char* sourcePtr = source.c_str();

    id = glCreateShader(shaderType);
    glShaderSource(id, 1, &sourcePtr, nullptr);
    glCompileShader(id);

    GLint success = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        char infoLog[1024];
        glGetShaderInfoLog(id, sizeof(infoLog), nullptr, infoLog);

        std::cout << "Shader compilation failed ("
            << shaderFile << "):\n"
            << infoLog << std::endl;

        glDeleteShader(id);
        id = 0;
    }
}

Shader::~Shader()
{
    if (id != 0)
    {
        glDeleteShader(id);
    }
}

void Shader::attachTo(GLuint shaderProgramId) const
{
    if (id != 0)
    {
        glAttachShader(shaderProgramId, id);
    }
}

void Shader::detachFrom(GLuint shaderProgramId) const
{
    if (id != 0)
    {
        glDetachShader(shaderProgramId, id);
    }
}

bool Shader::isValid() const
{
    return id != 0;
}