#pragma once

#include <glm/glm.hpp>

#include "Camera.h"
#include "Model.h"

class Scene {
private:
	Camera* camera;
	Model* model;

public:
	Scene();

	Scene(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, float aspectRatio);

	Model* getModel();

	Camera* getCamera();

	void cleanup(VkDevice& logicalDevice);
};

