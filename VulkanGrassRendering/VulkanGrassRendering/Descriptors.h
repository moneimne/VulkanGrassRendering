#pragma once

#include "Vertex.h"
#include "Blades.h"

#include <vulkan/vulkan.h>
#include <vector>

class Descriptors {
public:
	VkBuffer mvpBuffer;
	VkDeviceMemory mvpBufferMemory;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	VkBuffer bladesBuffer;
	VkDeviceMemory bladesBufferMemory;
	VkBuffer culledBladesBuffer;
	VkDeviceMemory culledBladesBufferMemory;
	VkBuffer timeBuffer;
	VkDeviceMemory timeBufferMemory;

	static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);

	static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool& commandPool, VkDevice& logicalDevice, VkQueue& graphicsQueue);

	static void createVertexBuffer(std::vector<Vertex>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue);

	static void createIndexBuffer(std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue);

	void createBladesBuffer(Blades* blades, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& computeQueue);

	void createCulledBladesBuffer(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& computeQueue);

	void createUniformBuffer(VkDeviceSize bufferSize, VkBuffer& buffer, VkDeviceMemory& bufferufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);

	void createMvpBuffer(VkDeviceSize bufferSize, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);

	void createTimeBuffer(VkDeviceSize bufferSize, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice);

	// Copy buffer data to image
	void copyBufferToImage(VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height);

	// Create a texture image
	void createTextureImage(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue);

	// Create an image view for the texture image
	void createTextureImageView(VkDevice& logicalDevice);

	// Create a sampler for the texture
	void createTextureSampler(VkDevice& logicalDevice);
};