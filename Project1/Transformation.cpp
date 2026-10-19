#include "Transformation.h"

#include <glm/gtc/matrix_transform.hpp>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <utility>


Translation::Translation(const glm::vec3& translation)
    : translation(translation)
{
}

void Translation::setTranslation(const glm::vec3& translation)
{
    this->translation = translation;
}

const glm::vec3& Translation::getTranslation() const
{
    return translation;
}

glm::mat4 Translation::getMatrix() const
{
    return glm::translate(
        glm::mat4(1.0f),
        translation
    );
}


Rotation::Rotation(float angle, const glm::vec3& axis) : angle(angle), axis(axis)
{
}

void Rotation::setAngle(float angle)
{
    this->angle = angle;
}

float Rotation::getAngle() const
{
    return angle;
}

void Rotation::setAxis(const glm::vec3& axis)
{
    this->axis = axis;
}

const glm::vec3& Rotation::getAxis() const
{
    return axis;
}

DynRotation::DynRotation(float speed, const glm::vec3& axis)
    : speed(speed), axis(axis)
{
}

glm::mat4 Rotation::getMatrix() const
{
    return glm::rotate(
        glm::mat4(1.0f),
        angle,
        axis
    );
}




glm::mat4 DynRotation::getMatrix() const
{
    float angle =
        speed * static_cast<float>(glfwGetTime());

    return glm::rotate(
        glm::mat4(1.0f),
        angle,
        axis
    );
}


Scale::Scale(const glm::vec3& scale)
    : scale(scale)
{
}

void Scale::setScale(const glm::vec3& scale)
{
    this->scale = scale;
}

const glm::vec3& Scale::getScale() const
{
    return scale;
}

glm::mat4 Scale::getMatrix() const
{
    return glm::scale(
        glm::mat4(1.0f),
        scale
    );
}


Transformation* CompositeTransformation::addTransformation(
    std::unique_ptr<Transformation> transformation)
{
    if (!transformation)
    {
        return nullptr;
    }

    Transformation* result = transformation.get();

    transformations.push_back(
        std::move(transformation)
    );

    return result;
}

void CompositeTransformation::clear()
{
    transformations.clear();
}

glm::mat4 CompositeTransformation::getMatrix() const
{
    glm::mat4 result(1.0f);

    for (const std::unique_ptr<Transformation>& transformation : transformations)
    {
        result =
            result * transformation->getMatrix();
    }

    return result;
}