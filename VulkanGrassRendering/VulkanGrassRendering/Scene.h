#pragma once

#include <glm/glm.hpp>
#include <chrono>

#include "Camera.h"
#include "Model.h"
#include "Blades.h"

using namespace std::chrono;

class Scene {
private:
	Camera* camera;
	Model* model;
	Blades* blades;

	high_resolution_clock::time_point startTime = high_resolution_clock::now();

public:
	float deltaTime = 0.0f;
	Scene();

	Scene(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, float aspectRatio);

	Model* getModel();

	Camera* getCamera();

	Blades* getBlades();

	void updateDeltaTime();

	void cleanup(VkDevice& logicalDevice);
};

