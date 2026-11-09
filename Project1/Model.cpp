#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Model.h"

int Model::currentFloatsPerVertex = 6;
int Model::currentColorOffset = 3;

void Model::setVertexFormat(
    int floatsPerVertex,
    int colorOffset)
{
    currentFloatsPerVertex = floatsPerVertex;
    currentColorOffset = colorOffset;
}

void Model::resetVertexFormat()
{
    currentFloatsPerVertex = 6;
    currentColorOffset = 3;
}

Model::Model(
    const float* data,
    std::size_t dataSizeBytes)
    : VBO(0),
    VAO(0),
    vertexCount(0)
{
    vertexCount = static_cast<GLsizei>(dataSizeBytes / (currentFloatsPerVertex * sizeof(float)));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(dataSizeBytes), data, GL_STATIC_DRAW);

    // 0: Pozice (vždy)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, currentFloatsPerVertex * sizeof(float), reinterpret_cast<void*>(0));

    if (currentFloatsPerVertex == 8)
    {
        // Texturovaný model: Normála (offset 3), UV (offset 6)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
    }
    else if (currentFloatsPerVertex == 9)
    {
        // Země/Slunce: Normála (offset 6), Barva (offset 3)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    }
    else
    {
        // Měsíc/Koule: Normála (offset 3)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
        glDisableVertexAttribArray(2);
        glVertexAttrib3f(2, 0.6f, 0.6f, 0.6f);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model::~Model()
{
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
    }
}

void Model::draw() const
{
    glBindVertexArray(VAO);

    glDrawArrays(
        GL_TRIANGLES,
        0,
        vertexCount
    );

    glBindVertexArray(0);
}