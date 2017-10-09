#pragma once

#include <vulkan/vulkan.h>

class Image {
public:
	// Create a Vulkan image
	static void createImage(VkDevice& logicalDevice, VkPhysicalDevice& physicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	// Handle layout transitions for images
	static void transitionImageLayout(VkDevice& logicalDevice, VkCommandPool& commandPool, VkQueue& graphicsQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	// Create an image view
	static VkImageView createImageView(VkDevice& logicalDevice, VkImage& image, VkFormat format, VkImageAspectFlags aspectFlags);
};

