#pragma once

#include "Shader.h"
#include "CameraObserver.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <string>

class ShaderProgram : public CameraObserver
{
private:
    Shader vertexShader;
    Shader fragmentShader;
    GLuint id;

    bool linkProgram();
    GLint getUniformLocation(const std::string& name) const;

public:
    ShaderProgram(
        const char* vertexShaderFile,
        const char* fragmentShaderFile);
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    void use() const;
    static void stop();

    bool isValid() const;
    GLuint getId() const;

    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, const glm::mat4& value) const;


    void setOptionalFloat(const std::string& name, float value) const;


    // Observer callback, invoked by Camera whenever its matrices change.
    void updateCamera(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) override;
};
