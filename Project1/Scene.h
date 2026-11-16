#pragma once

#include <memory>
#include <vector>
#include "Camera.h"
#include "Skybox.h"
#include "Texture.h"
#include "Transformation.h"

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
    std::unique_ptr<Skybox> skybox;

    Model* spawnModel = nullptr;
    ShaderProgram* spawnShader = nullptr;
    std::shared_ptr<Texture> spawnTexture = nullptr;
    int nextObjectID = 10;

    struct MovingEntity {
        DrawableObject* object;
        Translation* translation;
        glm::vec3 p0;
        glm::vec3 p1;
        float t = 0.0f;
        int direction = 1;
    };

    std::vector<MovingEntity> movingEntities;
    glm::vec3 tempPointP0 = glm::vec3(0.0f);
    bool hasFirstPoint = false;

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
    void setSkybox(const std::vector<std::string>& faces);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setSpawnTemplate(Model* model, ShaderProgram* shader, std::shared_ptr<Texture> texture = nullptr);
    void spawnObjectAt(const glm::vec3& position);

    void addPathPoint(const glm::vec3& point);
    void updateMovingObject(float deltaTime);
    void removeObjectByID(int id);
};
