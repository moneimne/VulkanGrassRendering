#include "Buffer.h"

#include <stdexcept>

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type");
}

void Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice) {
	// Create buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create vertex buffer");
	}

	// Query buffer's memory requirements
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(logicalDevice, buffer, &memRequirements);

	// Allocate memory in device
	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, physicalDevice);

	if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate vertex buffer");
	}

	// Associate allocated memory with vertex buffer
	vkBindBufferMemory(logicalDevice, buffer, bufferMemory, 0);
}

VkCommandBuffer beginSingleTimeCommands(VkCommandPool& commandPool, VkDevice& logicalDevice) {
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void endSingleTimeCommands(VkCommandPool& commandPool, VkCommandBuffer& commandBuffer, VkQueue& graphicsQueue, VkDevice logicalDevice) {
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(graphicsQueue);

	vkFreeCommandBuffers(logicalDevice, commandPool, 1, &commandBuffer);
}

void Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool& commandPool, VkDevice& logicalDevice, VkQueue& graphicsQueue) {
	// Create a temporary command buffer for the transfer operation
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, logicalDevice);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandPool, commandBuffer, graphicsQueue, logicalDevice);
}

void Buffer::createVertexBuffer(std::vector<Vertex>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
	// Create the staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBufferUsageFlags stagingUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VkMemoryPropertyFlags stagingProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	Buffer::createBuffer(bufferSize, stagingUsage, stagingProperties, stagingBuffer, stagingBufferMemory, logicalDevice, physicalDevice);

	// Fill the staging buffer
	void *data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	// Create the vertex buffer
	VkBufferUsageFlags vertexUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	VkMemoryPropertyFlags vertexFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	Buffer::createBuffer(bufferSize, vertexUsage, vertexFlags, vertexBuffer, vertexBufferMemory, logicalDevice, physicalDevice);

	// Copy data from staging to vertex buffer
	Buffer::copyBuffer(stagingBuffer, vertexBuffer, bufferSize, commandPool, logicalDevice, graphicsQueue);

	// No need for the staging buffer anymore
	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void Buffer::createIndexBuffer(std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
	// Create the staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();
	VkBufferUsageFlags stagingUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VkMemoryPropertyFlags stagingProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	createBuffer(bufferSize, stagingUsage, stagingProperties, stagingBuffer, stagingBufferMemory, logicalDevice, physicalDevice);

	// Fill the staging buffer
	void *data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	// Create the vertex buffer
	VkBufferUsageFlags indexUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
	VkMemoryPropertyFlags indexFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	createBuffer(bufferSize, indexUsage, indexFlags, indexBuffer, indexBufferMemory, logicalDevice, physicalDevice);

	// Copy data from staging to vertex buffer
	copyBuffer(stagingBuffer, indexBuffer, bufferSize, commandPool, logicalDevice, graphicsQueue);

	// No need for the staging buffer anymore
	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void Buffer::createBladesBuffer(Blades* blades, VkBuffer& bladesBuffer, VkDeviceMemory& bladesBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& computeQueue) {
	// Create the staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize bufferSize = NUM_BLADES * sizeof(Blade);
	VkBufferUsageFlags stagingUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VkMemoryPropertyFlags stagingProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	createBuffer(bufferSize, stagingUsage, stagingProperties, stagingBuffer, stagingBufferMemory, logicalDevice, physicalDevice);

	// Fill the staging buffer
	void *data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, blades->data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	// Create the storage buffer
	VkBufferUsageFlags storageUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	VkMemoryPropertyFlags storageFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	createBuffer(bufferSize, storageUsage, storageFlags, bladesBuffer, bladesBufferMemory, logicalDevice, physicalDevice);

	// Copy data from staging to storage buffer
	copyBuffer(stagingBuffer, bladesBuffer, bufferSize, commandPool, logicalDevice, computeQueue);

	// No need for the staging buffer anymore
	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void Buffer::createCulledBladesBuffer(VkBuffer& storageBuffer, VkDeviceMemory& storageBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& computeQueue) {
	// Create the storage buffer
	VkBufferUsageFlags storageUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	VkMemoryPropertyFlags storageFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	VkDeviceSize bufferSize = NUM_BLADES * sizeof(Blade);
	createBuffer(bufferSize, storageUsage, storageFlags, storageBuffer, storageBufferMemory, logicalDevice, physicalDevice);
}