#pragma once

#include <glad/gl.h>
#include <cstddef>

class Model
{
private:
    GLuint VBO;
    GLuint VAO;
    GLsizei vertexCount;

    static int currentFloatsPerVertex;
    static int currentColorOffset;

public:
    Model(
        const float* data,
        std::size_t dataSizeBytes
    );

    ~Model();

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    static void setVertexFormat(
        int floatsPerVertex,
        int colorOffset
    );

    static void resetVertexFormat();

    void draw() const;
};