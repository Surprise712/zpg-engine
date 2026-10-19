#include "Scene.h"

#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"

#include <utility>

Scene::Scene() = default;

Scene::~Scene()
{
    // Remove non-owning observer registrations before shaders are destroyed.
    for (const std::unique_ptr<ShaderProgram>& shaderProgram : shaderPrograms)
    {
        camera.removeObserver(shaderProgram.get());
    }
}

Model* Scene::addModel(std::unique_ptr<Model> model)
{
    if (!model)
    {
        return nullptr;
    }

    Model* result = model.get();
    models.push_back(std::move(model));
    return result;
}

ShaderProgram* Scene::addShaderProgram(
    std::unique_ptr<ShaderProgram> shaderProgram)
{
    if (!shaderProgram)
    {
        return nullptr;
    }

    ShaderProgram* result = shaderProgram.get();
    shaderPrograms.push_back(std::move(shaderProgram));
    camera.addObserver(result);
    return result;
}

DrawableObject* Scene::addObject(
    std::unique_ptr<DrawableObject> object)
{
    if (!object)
    {
        return nullptr;
    }

    DrawableObject* result = object.get();
    objects.push_back(std::move(object));
    return result;
}


void Scene::draw(float time) const
{
    for (const auto& shader : shaderPrograms)
    {
        shader->use();
        shader->setOptionalFloat("time", time);
    }

    for (const auto& object : objects)
    {
        object->draw();
    }
}

void Scene::rotateCamera(float deltaAlpha, float deltaFi)
{
    camera.rotate(deltaAlpha, deltaFi);
}

void Scene::setAspectRatio(float aspectRatio)
{
    camera.setAspectRatio(aspectRatio);
}


void Scene::moveCamera(float forward, float sideways)
{
    camera.move(forward, sideways);
}