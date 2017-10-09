#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

#include "Vertex.h"
#include "Scene.h"
#include "Descriptors.h"

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;
	int computeFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0 && computeFamily >= 0;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct MvpBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

class Renderer {
public:
	void run();

private:
	GLFWwindow* window;
	VkInstance instance;
	VkDebugReportCallbackEXT callback;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice logicalDevice;

	VkQueue graphicsQueue;
	VkQueue computeQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;

	VkDescriptorSetLayout graphicsDescriptorSetLayout;
	VkPipelineLayout graphicsPipelineLayout;
	VkPipeline graphicsPipeline;

	VkDescriptorSetLayout grassDescriptorSetLayout;
	VkPipelineLayout grassPipelineLayout;
	VkPipeline grassPipeline;

	VkDescriptorSetLayout computeDescriptorSetLayout;
	VkPipelineLayout computePipelineLayout;
	VkPipeline computePipeline;

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	VkCommandPool computeCommandPool;
	VkCommandBuffer computeCommandBuffer;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;

	VkDescriptorPool descriptorPool;
	VkDescriptorSet graphicsDescriptorSet;
	VkDescriptorSet grassDescriptorSet;
	VkDescriptorSet computeDescriptorSet;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	Descriptors descriptors;

	Scene scene;
	
	// Initialize the GLFW window
	void initWindow();
	
	// Window resize callback
	static void onWindowResized(GLFWwindow* window, int width, int height);

	// Mouse click callback
	static void onMouseClick(GLFWwindow* window, int button, int action, int mods);

	// Mouse move callback
	static void onMouseMove(GLFWwindow* window, double xPosition, double yPosition);

	// Create an instance, which is the connection between the application and the Vulkan library
	void createInstance();

	// Allow messages to be received from validation layers through a callback
	void setupDebugCallback();

	// Create the surface that allows Bulkan to interface with the window system on its own
	void createSurface();

	// Choose a graphics card that suits our needs
	void pickPhysicalDevice();

	// Create the logical device, which will allow us to interface with the physical device
	void createLogicalDevice();

	// Create the swap chain, which stores the queue of images to be rendered to the screen
	void createSwapChain();

	// Create the image views, which describe how to access the images in the swap chain
	void createImageViews();

	// Create the render pass, which specifies the framebuffer attachments that will be used
	void createRenderPass();

	// Create the descriptor set layout, which describes the details about every descriptor binding used in shaders
	void createGraphicsDescriptorSetLayout();

	// Create the graphics pipeline
	void createGraphicsPipeline();

	// Create the grass descriptor set layout
	void createGrassDescriptorSetLayout();

	// Create the grass graphics pipeline
	void createGrassPipeline();

	// Create the descriptor set layout for the compute pipeline
	void createComputeDescriptorSetLayout();

	// Create the compute pipeline
	void createComputePipeline();

	// Create the framebuffers, one for each image in the swap chain
	void createFramebuffers();

	// Create a command pool, which manages the memory that is used to store buffers and command buffers
	void createCommandPool();

	// Create a command pool for the compute pipeline
	void createComputeCommandPool();

	// Create resources for depth buffer
	void createDepthResources();

	// Create descriptor pool, in which descriptor sets can be allocated
	void createDescriptorPool();

	// Create the descriptor set
	void createGraphicsDescriptorSet();

	// Create the grass descriptor set
	void createGrassDescriptorSet();

	// Create the compute descriptor set
	void createComputeDescriptorSet();

	// Update the uniform buffer
	void updateMvpBuffer();

	// Update the time buffer
	void updateTimeBuffer();

	// Create the command buffers, which are used to record drawing commands to be used in the pipeline
	void createCommandBuffers(Model& model);

	// Create the compute command buffer
	void createComputeCommandBuffer();

	// Create the semaphores that will help synchronize the queue operations of draw commands and presentation
	void createSemaphores();

	void initVulkan();

	void createScene();

	// Draw contents on screen
	void drawFrame();

	void mainLoop();

	// Recreate the swap chain if the curren one is no longer viable
	void recreateSwapChain();

	// Clean the swap chain on recreation
	void cleanupSwapChain();

	void cleanup();
};

