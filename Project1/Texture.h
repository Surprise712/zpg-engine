#pragma once
#include <glad/gl.h>

class Texture {
private:
    GLuint id;

public:
    Texture(const char* filepath);
    void bind(int textureUnit = 0) const;
};