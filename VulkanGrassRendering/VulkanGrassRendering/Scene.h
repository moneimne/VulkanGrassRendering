#pragma once

#include <glm/glm.hpp>
#include <chrono>

#include "Camera.h"
#include "Model.h"
#include "Blades.h"

using namespace std::chrono;

struct Time {
	float deltaTime = 0.0f;
	float totalTime = 0.0f;
};

class Scene {
private:
	Camera* camera;
	Model* model;
	Blades* blades;

	high_resolution_clock::time_point startTime = high_resolution_clock::now();

public:
	Time time;
	Scene();

	Scene(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, float aspectRatio);

	Model* getModel();

	Camera* getCamera();

	Blades* getBlades();

	void updateTime();

	void cleanup(VkDevice& logicalDevice);
};

