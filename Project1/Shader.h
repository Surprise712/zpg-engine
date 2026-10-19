#pragma once

#include <glad/gl.h>

class Shader
{
private:
    GLuint id;

public:
    Shader(GLenum shaderType, const char* shaderFile);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void attachTo(GLuint shaderProgramId) const;
    void detachFrom(GLuint shaderProgramId) const;
    bool isValid() const;
};