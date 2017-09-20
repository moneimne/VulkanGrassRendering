#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
#include "Model.h"
#include "Blades.h"

class Scene {
private:
	Camera* camera;
	Model* model;
	Blades* blades;

public:
	Scene();

	Scene(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, float aspectRatio);

	Model* getModel();

	Camera* getCamera();

	Blades* getBlades();

	void cleanup(VkDevice& logicalDevice);
};

