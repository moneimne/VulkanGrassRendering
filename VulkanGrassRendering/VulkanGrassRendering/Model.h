#pragma once

#include <vector>

#include "Vertex.h"

const float PLANE_DIM = 15.0f;

class Model {
private:
	std::vector<Vertex> vertices;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	std::vector<uint32_t> indices;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	glm::mat4 modelMatrix;

public:
	Model();

	Model(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue);

	std::vector<Vertex> getVertices();

	VkBuffer getVertexBuffer();

	std::vector<uint32_t> getIndices();

	VkBuffer getIndexBuffer();

	glm::mat4 getModelMatrix();

	void cleanup(VkDevice& logicalDevice);
};

