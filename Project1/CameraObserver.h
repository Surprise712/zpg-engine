#pragma once

#include <glm/glm.hpp>

class CameraObserver
{
public:
	virtual ~CameraObserver() = default;

	virtual void updateCamera(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) = 0;
};