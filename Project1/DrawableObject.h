#pragma once

#include "Transformation.h"

#include <memory>

class Model;
class ShaderProgram;

class DrawableObject
{
private:
    Model* model;
    ShaderProgram* shaderProgram;
    std::unique_ptr<Transformation> transformation;

public:
    DrawableObject(
        Model* model,
        ShaderProgram* shaderProgram,
        std::unique_ptr<Transformation> transformation);

    ~DrawableObject() = default;

    DrawableObject(const DrawableObject&) = delete;
    DrawableObject& operator=(const DrawableObject&) = delete;

    void draw(const glm::vec3& cameraPosition) const;

    Transformation* getTransformation();
    const Transformation* getTransformation() const;

    void setTransformation(
        std::unique_ptr<Transformation> transformation);
};
