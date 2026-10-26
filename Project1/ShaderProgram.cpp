#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ShaderProgram::ShaderProgram(
    const char* vertexShaderFile,
    const char* fragmentShaderFile)
    : vertexShader(GL_VERTEX_SHADER, vertexShaderFile),
    fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderFile),
    id(0)
{
    linkProgram();
}

ShaderProgram::~ShaderProgram()
{
    if (id != 0)
    {
        glDeleteProgram(id);
    }
}

bool ShaderProgram::linkProgram()
{
    if (!vertexShader.isValid() || !fragmentShader.isValid())
    {
        std::cout << "Shader program cannot be linked because a shader is invalid."
            << std::endl;
        return false;
    }

    id = glCreateProgram();

    vertexShader.attachTo(id);
    fragmentShader.attachTo(id);

    glLinkProgram(id);

    GLint success = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        char infoLog[1024];
        glGetProgramInfoLog(id, sizeof(infoLog), nullptr, infoLog);

        std::cout << "Shader program linking failed:\n"
            << infoLog << std::endl;

        glDeleteProgram(id);
        id = 0;
        return false;
    }

    vertexShader.detachFrom(id);
    fragmentShader.detachFrom(id);

    return true;
}

void ShaderProgram::use() const
{
    glUseProgram(id);
}

void ShaderProgram::stop()
{
    glUseProgram(0);
}

bool ShaderProgram::isValid() const
{
    return id != 0;
}

GLuint ShaderProgram::getId() const
{
    return id;
}

GLint ShaderProgram::getUniformLocation(const std::string& name) const
{
    GLint location = glGetUniformLocation(id, name.c_str());

    //if (location == -1)
    //{
    //    std::cout << "Uniform not found: " << name << std::endl;
    //}

    return location;
}

void ShaderProgram::setUniform(
    const std::string& name,
    float value) const
{
    GLint location = getUniformLocation(name);

    if (location != -1)
    {
        glUniform1f(location, value);
    }
}

void ShaderProgram::setUniform(
    const std::string& name,
    int value) const
{
    GLint location = getUniformLocation(name);

    if (location != -1)
    {
        glUniform1i(location, value);
    }
}

void ShaderProgram::setUniform(
    const std::string& name,
    const glm::vec3& value) const
{
    GLint location = getUniformLocation(name);

    if (location != -1)
    {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void ShaderProgram::setUniform(
    const std::string& name,
    const glm::mat4& value) const
{
    GLint location = getUniformLocation(name);

    if (location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
}




void ShaderProgram::setOptionalFloat(const std::string& name, float value) const
{
    if (id == 0) return;

    GLint location = glGetUniformLocation(id, name.c_str());

    if (location != -1)
        glUniform1f(location, value);
}



void ShaderProgram::updateCamera(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    if (!isValid())
    {
        return;
    }

    // OpenGL uniforms belong to the currently bound shader program.
    use();
    setUniform("viewMatrix", viewMatrix);
    setUniform("projectionMatrix", projectionMatrix);
}