#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Camera.h"

class Skybox {
private:
    unsigned int VAO;
    unsigned int VBO;
    unsigned int cubemapTexture;

    // Identifikátor shader programu (nebo instance tvé třídy Shader)
    ShaderProgram* shaderProgram;

    // Pomocné inicializační metody, aby konstruktor nebyl obří
    void setupGeometry();
    unsigned int loadCubemap(const std::vector<std::string>& faces);
public:
    // Konstruktor přijímá vektor 6 cest k texturám (+X, -X, +Y, -Y, +Z, -Z)
    Skybox(const std::vector<std::string>& faces, ShaderProgram* shaderProgram);

    // Destruktor pro úklid paměti na GPU (smazání VAO, VBO, textury)
    ~Skybox();

    // Hlavní vykreslovací metoda. Přijímá matice (nebo referenci na tvou Kameru)
    void draw(const glm::mat4& view, const glm::mat4& projection);


};