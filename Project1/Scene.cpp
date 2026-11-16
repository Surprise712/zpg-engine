#include "Scene.h"

#include "DrawableObject.h"
#include "Model.h"
#include "ShaderProgram.h"

#include <utility>
#include <algorithm>

void Scene::addPathPoint(const glm::vec3& point) {
    if (!spawnModel || !spawnShader) return;

    if (!hasFirstPoint) {
        // 1. kliknutí: uložíme počáteční bod P0
        tempPointP0 = point;
        hasFirstPoint = true;
        printf("Bod P0 nastaven na: [%.2f, %.2f, %.2f]\n", tempPointP0.x, tempPointP0.y, tempPointP0.z);
    }
    else {
        // 2. kliknutí: uložíme koncový bod P1 a přidáme další pohybující se židli do seznamu
        glm::vec3 pointP1 = point;
        hasFirstPoint = false;
        printf("Bod P1 nastaven na: [%.2f, %.2f, %.2f] -> Pridan dalsi pohybujici se objekt!\n", pointP1.x, pointP1.y, pointP1.z);

        auto compTrans = std::make_unique<CompositeTransformation>();

        auto trans = std::make_unique<Translation>(tempPointP0);
        Translation* transPtr = trans.get();
        compTrans->addTransformation(std::move(trans));

        auto newObj = std::make_unique<DrawableObject>(spawnModel, spawnShader, std::move(compTrans));
        if (spawnTexture) {
            newObj->setTexture(spawnTexture);
        }
        newObj->setID(nextObjectID++);

        DrawableObject* objPtr = addObject(std::move(newObj));

        // Uložíme nový pohybující se objekt do vektoru (předchozí běží dál)
        movingEntities.push_back({ objPtr, transPtr, tempPointP0, pointP1, 0.0f, 1 });
    }
}

void Scene::updateMovingObject(float deltaTime) {
    float speed = 0.5f;

    for (auto& entity : movingEntities) {
        entity.t += entity.direction * speed * deltaTime;

        if (entity.t >= 1.0f) {
            entity.t = 1.0f;
            entity.direction = -1;
        }
        else if (entity.t <= 0.0f) {
            entity.t = 0.0f;
            entity.direction = 1;
        }

        // P(t) = P0 + t * (P1 - P0)
        glm::vec3 currentPos = entity.p0 + entity.t * (entity.p1 - entity.p0);
        entity.translation->setTranslation(currentPos);
    }
}

void Scene::removeObjectByID(int id) {
    if (id <= 0) return;

    // Nejdříve objekt odebereme ze seznamu pohybujících se entit (pokud tam je)
    movingEntities.erase(
        std::remove_if(movingEntities.begin(), movingEntities.end(),
            [id](const MovingEntity& entity) {
                return entity.object != nullptr && entity.object->getID() == id;
            }),
        movingEntities.end()
    );

    // Poté jej smažeme ze scény
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [id](const std::unique_ptr<DrawableObject>& obj) {
                return obj->getID() == id;
            }),
        objects.end()
    );
    printf("Objekt s ID %d byl smazan ze sceny.\n", id);
}

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
    if (skybox) {
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        skybox->draw(camera.getViewMatrix(), camera.getProjectionMatrix());
    }
    // Tento cyklus je v pořádku - updatuje čas pro všechny shadery najednou.
    // (OpenGL si uniformní proměnné pamatuje uvnitř každého shader programu)
    for (const auto& shader : shaderPrograms)
    {
        shader->use();
        shader->setOptionalFloat("time", time);
    }

    for (const auto& object : objects)
    {
        glStencilFunc(GL_ALWAYS, object->getID(), 0xFF);
        // Změněno z camera->Position na camera.Position
        // (Případně použijte camera.getPosition(), pokud je proměnná privátní a máte na ni getter)
        object->draw(camera.getPosition());
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

void Scene::setSkybox(const std::vector<std::string>& faces) {
    ShaderProgram* skyboxShader = addShaderProgram(
        std::make_unique<ShaderProgram>("../shaders/skybox.vert", "../shaders/skybox.frag")
    );
    skybox = std::make_unique<Skybox>(faces, skyboxShader);
}

glm::mat4 Scene::getViewMatrix() const { return camera.getViewMatrix(); }
glm::mat4 Scene::getProjectionMatrix() const { return camera.getProjectionMatrix(); }

void Scene::setSpawnTemplate(Model* model, ShaderProgram* shader, std::shared_ptr<Texture> texture) {
    spawnModel = model;
    spawnShader = shader;
    spawnTexture = texture;
}

void Scene::spawnObjectAt(const glm::vec3& position) {
    if (!spawnModel || !spawnShader) return;

    auto trans = std::make_unique<CompositeTransformation>();
    trans->addTransformation(std::make_unique<Translation>(position));

    auto newObj = std::make_unique<DrawableObject>(spawnModel, spawnShader, std::move(trans));
    if (spawnTexture) {
        newObj->setTexture(spawnTexture);
    }
    newObj->setID(nextObjectID++);
    addObject(std::move(newObj));
}
