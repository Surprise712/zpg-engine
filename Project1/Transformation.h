#pragma once

#include <glm/glm.hpp>

#include <memory>
#include <vector>


class Transformation
{
public:
    virtual ~Transformation() = default;

    virtual glm::mat4 getMatrix() const = 0;
};


class Translation : public Transformation
{
private:
    glm::vec3 translation;

public:
    Translation(const glm::vec3& translation);

    void setTranslation(const glm::vec3& translation);
    const glm::vec3& getTranslation() const;

    glm::mat4 getMatrix() const override;
};


class Rotation : public Transformation
{
private:
    float angle;
    glm::vec3 axis;

public:
    Rotation(
        float angle,
        const glm::vec3& axis
    );

    void setAngle(float angle);
    float getAngle() const;

    void setAxis(const glm::vec3& axis);
    const glm::vec3& getAxis() const;

    glm::mat4 getMatrix() const override;
};


class DynRotation : public Transformation
{
private:
    float speed;
    glm::vec3 axis;

public:
    DynRotation(
        float speed,
        const glm::vec3& axis
    );

    glm::mat4 getMatrix() const override;
};


class Scale : public Transformation
{
private:
    glm::vec3 scale;

public:
    Scale(const glm::vec3& scale);

    void setScale(const glm::vec3& scale);
    const glm::vec3& getScale() const;

    glm::mat4 getMatrix() const override;
};


class CompositeTransformation : public Transformation
{
private:
    std::vector<std::unique_ptr<Transformation>> transformations;

public:
    Transformation* addTransformation(
        std::unique_ptr<Transformation> transformation
    );

    void clear();

    glm::mat4 getMatrix() const override;
};