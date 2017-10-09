#include "Model.h"
#include "Descriptors.h"

Model::Model() {
}

Model::Model(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
	vertices = {
		{ { -2.0f, 0.0f, 2.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
		{ { 2.0f, 0.0f, 2.0f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } },
		{ { 2.0f, 0.0f, -2.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },
		{ { -2.0f, 0.0f, -2.0f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } }
	};

	Descriptors::createVertexBuffer(vertices, vertexBuffer, vertexBufferMemory, logicalDevice, physicalDevice, commandPool, graphicsQueue);

	indices = {
		0, 1, 2, 2, 3, 0
	};

	Descriptors::createIndexBuffer(indices, indexBuffer, indexBufferMemory, logicalDevice, physicalDevice, commandPool, graphicsQueue);

	modelMatrix = glm::mat4(1.0f);
}

std::vector<Vertex> Model::getVertices() {
	return vertices;
}

VkBuffer Model::getVertexBuffer() {
	return vertexBuffer;
}

std::vector<uint32_t> Model::getIndices() {
	return indices;
}

VkBuffer Model::getIndexBuffer() {
	return indexBuffer;
}

glm::mat4 Model::getModelMatrix() {
	return modelMatrix;
}

void Model::cleanup(VkDevice& logicalDevice) {
	vkDestroyBuffer(logicalDevice, indexBuffer, nullptr);
	vkFreeMemory(logicalDevice, indexBufferMemory, nullptr);

	vkDestroyBuffer(logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(logicalDevice, vertexBufferMemory, nullptr);
}