#pragma once

#include <glm/glm.hpp>
#include <vector>

class CameraObserver;

// Subject (Observer pattern): the camera notifies its registered shader programs
// whenever its view or projection matrix changes.
class Camera
{
private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    glm::vec3 eye;
    glm::vec3 target;
    glm::vec3 up;

    float alpha;
    float fi;

    // Non-owning pointers: Scene owns the shader programs.
    std::vector<CameraObserver*> observers;

    void updateViewMatrix();
    void notifyObservers() const;

public:
    Camera();

    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    glm::mat4 getCamera() const;
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void addObserver(CameraObserver* observer);
    void removeObserver(CameraObserver* observer);

    void rotate(float deltaAlpha, float deltaFi);
    void setAspectRatio(float aspectRatio);

    void move(float forward, float sideways);

    glm::vec3 getPosition() const;

};
