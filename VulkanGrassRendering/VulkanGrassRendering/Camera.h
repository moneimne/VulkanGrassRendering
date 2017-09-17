#pragma once

#include <glm/glm.hpp>

class Camera {
private:
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	float r, theta, phi;

public:
	Camera();
	Camera(float aspectRatio);

	void updateOrbit(double deltaX, double deltaY, double deltaZ);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
};

