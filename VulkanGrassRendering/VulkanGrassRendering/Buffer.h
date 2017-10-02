#pragma once

#include "Vertex.h"
#include "Blades.h"

#include <vulkan/vulkan.h>
#include <vector>

class Buffer {
public:
	static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);

	static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool& commandPool, VkDevice& logicalDevice, VkQueue& graphicsQueue);

	static void createVertexBuffer(std::vector<Vertex>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue);

	static void createIndexBuffer(std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue);

	static void createBladesBuffer(Blades* blades, VkBuffer& bladesBuffer, VkDeviceMemory& bladesBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& computeQueue);

	static void createCulledBladesBuffer(VkBuffer& storageBuffer, VkDeviceMemory& storageBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& computeQueue);
};