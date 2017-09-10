#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vector>

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
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
	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	
	// Initialize the GLFW window
	void initWindow();

	static void onWindowResized(GLFWwindow* window, int width, int height);

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

	// Create the graphics pipeline
	void createGraphicsPipeline();

	// Create the framebuffers, one for each image in the swap chain
	void createFramebuffers();

	// Create a command pool, which manages the memory that is used to store buffers and command buffers
	void createCommandPool();

	// Create the command buffers, which are used to record drawing commands to be used in the pipeline
	void createCommandBuffers();

	// Create the semaphores that will help synchronize the queue operations of draw commands and presentation
	void createSemaphores();

	void initVulkan();

	// Draw contents on screen
	void drawFrame();

	void mainLoop();

	// Recreate the swap chain if the curren one is no longer viable
	void recreateSwapChain();

	// Clean the swap chain on recreation
	void cleanupSwapChain();

	void cleanup();
};

