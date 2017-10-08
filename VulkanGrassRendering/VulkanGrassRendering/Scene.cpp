#include "Scene.h"

Scene::Scene() {
	camera = new Camera(1.0f);
	model = new Model();
	blades = new Blades(model, NUM_BLADES);
}

Scene::Scene(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, float aspectRatio) {
	camera = new Camera(aspectRatio);
	model = new Model(logicalDevice, physicalDevice, commandPool, graphicsQueue);
	blades = new Blades(model, NUM_BLADES);
}

Model* Scene::getModel() {
	return model;
}

Camera* Scene::getCamera() {
	return camera;
}

Blades* Scene::getBlades() {
	return blades;
}

void Scene::updateDeltaTime() {
	high_resolution_clock::time_point currentTime = high_resolution_clock::now();
	duration<float> nextDeltaTime = duration_cast<duration<float>>(currentTime - startTime);
	startTime = currentTime;

	deltaTime = nextDeltaTime.count();
}

void Scene::cleanup(VkDevice& logicalDevice) {
	model->cleanup(logicalDevice);
}