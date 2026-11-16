#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <utility>

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram, std::unique_ptr<Transformation> transformation)
    : model(model), shaderProgram(shaderProgram), transformation(std::move(transformation))
{
}

// Přidán parametr pro pozici kamery (nebo můžete předávat celou instanci Camera)
void DrawableObject::draw(const glm::vec3& cameraPosition) const
{
    if (model == nullptr || shaderProgram == nullptr)
    {
        return;
    }

    // 1. Aktivace konkrétního shaderu patřícího tomuto objektu
    shaderProgram->use();

    // 2. Výpočet a odeslání matice modelu
    glm::mat4 modelMatrix(1.0f);
    if (transformation)
    {
        modelMatrix = transformation->getMatrix();
    }
    shaderProgram->setUniform("modelMatrix", modelMatrix);

    // 3. Odeslání pozice kamery pro výpočet odlesků ve fragment shaderu
    // (Předpokládám, že vaše třída ShaderProgram má přetíženou metodu setUniform pro vec3)
    shaderProgram->setUniform("viewPosition", cameraPosition);

    if (texture) {
        shaderProgram->use();
        texture->bind(0);
        GLint uniformID = glGetUniformLocation(shaderProgram->getId(), "textureUnitID");
        glUniform1i(uniformID, 0);
    }

    // 4. Samotné vykreslení
    model->draw();
}

Transformation* DrawableObject::getTransformation()
{
    return transformation.get();
}

const Transformation* DrawableObject::getTransformation() const
{
    return transformation.get();
}

void DrawableObject::setTransformation(std::unique_ptr<Transformation> transformation)
{
    this->transformation = std::move(transformation);
}

void DrawableObject::setTexture(std::shared_ptr<Texture> texture) {
    this->texture = std::move(texture);
}

void DrawableObject::setID(int newID) { this->id = newID; }
int DrawableObject::getID() const { return this->id; }