#include "DrawableObject.h"

#include "Model.h"
#include "ShaderProgram.h"

#include <glm/glm.hpp>

#include <utility>

DrawableObject::DrawableObject(Model* model, ShaderProgram* shaderProgram,std::unique_ptr<Transformation> transformation) : model(model), shaderProgram(shaderProgram),transformation(std::move(transformation))
{



}

void DrawableObject::draw() const
{
    if (model == nullptr || shaderProgram == nullptr)
    {
        return;
    }

    shaderProgram->use();

    glm::mat4 modelMatrix(1.0f);

    if (transformation)
    {
        modelMatrix = transformation->getMatrix();
    }

    shaderProgram->setUniform("modelMatrix", modelMatrix);
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
