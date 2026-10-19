#include "Camera.h"
#include "CameraObserver.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

Camera::Camera()
    : viewMatrix(1.0f),
      projectionMatrix(glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f)),
      eye(0.0f, 0.0f, 3.0f),
      target(0.0f, 0.0f, -1.0f),
      up(0.0f, 1.0f, 0.0f),
      alpha(glm::radians(90.0f)),
      fi(glm::radians(-90.0f))
{
    updateViewMatrix();
}

glm::mat4 Camera::getCamera() const
{
    return viewMatrix;
}

glm::mat4 Camera::getViewMatrix() const
{
    return viewMatrix;
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return projectionMatrix;
}

void Camera::addObserver(CameraObserver* observer)
{
    if (observer == nullptr ||
        std::find(observers.begin(), observers.end(), observer) != observers.end())
    {
        return;
    }

    observers.push_back(observer);

    observer->updateCamera(viewMatrix, projectionMatrix);
}

void Camera::removeObserver(CameraObserver* observer)
{
    observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Camera::notifyObservers() const
{
    for (CameraObserver* observer : observers)
    {
        observer->updateCamera(viewMatrix, projectionMatrix);
    }
}

void Camera::updateViewMatrix()
{
    target.x = std::sin(alpha) * std::cos(fi);
    target.z = std::sin(alpha) * std::sin(fi);
    target.y = std::cos(alpha);

    viewMatrix = glm::lookAt(eye, eye + target, up);
}

void Camera::rotate(float deltaAlpha, float deltaFi)
{
    if (deltaAlpha == 0.0f && deltaFi == 0.0f)
    {
        return;
    }

    alpha = glm::clamp(alpha + deltaAlpha,
                       glm::radians(1.0f), glm::radians(179.0f));
    fi += deltaFi;

    updateViewMatrix();
    notifyObservers();
}

void Camera::setAspectRatio(float aspectRatio)
{
    if (aspectRatio <= 0.0f)
    {
        return;
    }

    projectionMatrix = glm::perspective(glm::radians(60.0f), aspectRatio, 0.1f, 100.0f);
    notifyObservers();
}




void Camera::move(float forward, float sideways)
{
    if (forward == 0.0f && sideways == 0.0f) return;

    glm::vec3 direction = glm::normalize(target);
    glm::vec3 right = glm::normalize(glm::cross(direction, up));

    eye += direction * forward + right * sideways;

    updateViewMatrix();
    notifyObservers();
}


