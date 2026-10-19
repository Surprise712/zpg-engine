#pragma once

#include <memory>
#include <vector>
#include "Camera.h"

class DrawableObject;
class Model;
class ShaderProgram;

class Scene
{
private:
    std::vector<std::unique_ptr<Model>> models;
    std::vector<std::unique_ptr<ShaderProgram>> shaderPrograms;
    std::vector<std::unique_ptr<DrawableObject>> objects;

    Camera camera;

public:
    Scene();
    ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Model* addModel(std::unique_ptr<Model> model);

    ShaderProgram* addShaderProgram(
        std::unique_ptr<ShaderProgram> shaderProgram);

    DrawableObject* addObject(
        std::unique_ptr<DrawableObject> object);

    void draw(float time) const;


    void rotateCamera(float deltaAlpha, float deltaFi);
    void setAspectRatio(float aspectRatio);

    void moveCamera(float forward, float sideways);

};
