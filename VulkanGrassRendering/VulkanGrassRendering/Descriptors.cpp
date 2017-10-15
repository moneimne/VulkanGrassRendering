#include "Descriptors.h"
#include "Image.h"
#include "Utilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>

void Descriptors::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice) {
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

void Descriptors::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool& commandPool, VkDevice& logicalDevice, VkQueue& graphicsQueue) {
	// Create a temporary command buffer for the transfer operation
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, logicalDevice);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	endSingleTimeCommands(commandPool, commandBuffer, graphicsQueue, logicalDevice);
}

void Descriptors::createVertexBuffer(std::vector<Vertex>& vertices, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
	// Create the staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
	VkBufferUsageFlags stagingUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VkMemoryPropertyFlags stagingProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	createBuffer(bufferSize, stagingUsage, stagingProperties, stagingBuffer, stagingBufferMemory, logicalDevice, physicalDevice);

	// Fill the staging buffer
	void *data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	// Create the vertex buffer
	VkBufferUsageFlags vertexUsage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	VkMemoryPropertyFlags vertexFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	createBuffer(bufferSize, vertexUsage, vertexFlags, vertexBuffer, vertexBufferMemory, logicalDevice, physicalDevice);

	// Copy data from staging to vertex buffer
	copyBuffer(stagingBuffer, vertexBuffer, bufferSize, commandPool, logicalDevice, graphicsQueue);

	// No need for the staging buffer anymore
	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void Descriptors::createIndexBuffer(std::vector<uint32_t>& indices, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
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

void Descriptors::createBladesBuffer(Blades* blades, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& computeQueue) {
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

void Descriptors::createCulledBladesBuffer(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& computeQueue) {
	// Create the storage buffer
	VkBufferUsageFlags storageUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	VkMemoryPropertyFlags storageFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	VkDeviceSize bufferSize = NUM_BLADES * sizeof(Blade);
	createBuffer(bufferSize, storageUsage, storageFlags, culledBladesBuffer, culledBladesBufferMemory, logicalDevice, physicalDevice);
}

void Descriptors::createUniformBuffer(VkDeviceSize bufferSize, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice) {
	// Create uniform buffer
	VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	createBuffer(bufferSize, usage, properties, buffer, bufferMemory, logicalDevice, physicalDevice);
}

void Descriptors::createMvpBuffer(VkDeviceSize bufferSize, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice) {
	createUniformBuffer(bufferSize, mvpBuffer, mvpBufferMemory, logicalDevice, physicalDevice);
}

void Descriptors::createTimeBuffer(VkDeviceSize bufferSize, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice) {
	createUniformBuffer(bufferSize, timeBuffer, timeBufferMemory, logicalDevice, physicalDevice);
}

void Descriptors::createNumBladesBuffer(VkDeviceSize bufferSize, VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice) {
	VkBufferUsageFlags storageUsage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
	VkMemoryPropertyFlags storageFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	createBuffer(bufferSize, storageUsage, storageFlags, numBladesBuffer, numBladesBufferMemory, logicalDevice, physicalDevice);
}

void Descriptors::copyBufferToImage(VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkBuffer& buffer, VkImage& image, uint32_t width, uint32_t height) {
	VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool, logicalDevice);

	// Specify which part of the buffer is going to be copied to which part of the image
	VkBufferImageCopy region = {};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;

	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;

	region.imageOffset = { 0, 0, 0 };
	region.imageExtent = { width, height, 1 };

	vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	endSingleTimeCommands(commandPool, commandBuffer, graphicsQueue, logicalDevice);
}

void Descriptors::createTextureImage(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue) {
	// Load the image
	int texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load("Textures/grass.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	if (!pixels) {
		throw std::runtime_error("Failed to load texture image");
	}

	// Create staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkBufferUsageFlags stagingUsage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	VkMemoryPropertyFlags stagingProperties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
	createBuffer(imageSize, stagingUsage, stagingProperties, stagingBuffer, stagingBufferMemory, logicalDevice, physicalDevice);

	// Copy pixel values to the buffer
	void* data;
	vkMapMemory(logicalDevice, stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(logicalDevice, stagingBufferMemory);

	// Free pixel array
	stbi_image_free(pixels);

	// Create Vulkan image
	VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
	VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
	VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	Image::createImage(logicalDevice, physicalDevice, texWidth, texHeight, format, tiling, usage, properties, textureImage, textureImageMemory);

	// Copy the staging buffer to the texture image
	// --> First need to transition the texture image to VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
	Image::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	copyBufferToImage(logicalDevice, commandPool, graphicsQueue, stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

	// Transition texture image for shader access
	Image::transitionImageLayout(logicalDevice, commandPool, graphicsQueue, textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	// No need for stagin buffer anymore
	vkDestroyBuffer(logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(logicalDevice, stagingBufferMemory, nullptr);
}

void Descriptors::createTextureImageView(VkDevice& logicalDevice) {
	textureImageView = Image::createImageView(logicalDevice, textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT);
}

void Descriptors::createTextureSampler(VkDevice& logicalDevice) {
	// --- Specify all filters and transformations ---
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;

	// Interpolation of texels that are magnified or minified
	samplerInfo.magFilter = VK_FILTER_LINEAR;
	samplerInfo.minFilter = VK_FILTER_LINEAR;

	// Addressing mode
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

	// Anisotropic filtering
	samplerInfo.anisotropyEnable = VK_TRUE;
	samplerInfo.maxAnisotropy = 16;

	// Border color
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;

	// Choose coordinate system for addressing texels --> [0, 1) here
	samplerInfo.unnormalizedCoordinates = VK_FALSE;

	// Comparison function used for filtering operations
	samplerInfo.compareEnable = VK_FALSE;
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

	// Mipmapping
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	samplerInfo.mipLodBias = 0.0f;
	samplerInfo.minLod = 0.0f;
	samplerInfo.maxLod = 0.0f;

	if (vkCreateSampler(logicalDevice, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create texture sampler");
	}
}