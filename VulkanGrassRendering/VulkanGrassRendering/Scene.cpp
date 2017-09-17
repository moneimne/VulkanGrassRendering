#include "Scene.h"

Scene::Scene() {
	camera = new Camera(1.0f);
	model = new Model();
}

Scene::Scene(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, float aspectRatio) {
	camera = new Camera(aspectRatio);
	model = new Model(logicalDevice, physicalDevice, commandPool, graphicsQueue);
}

Model* Scene::getModel() {
	return model;
}

Camera* Scene::getCamera() {
	return camera;
}

void Scene::cleanup(VkDevice& logicalDevice) {
	model->cleanup(logicalDevice);
}