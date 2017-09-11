#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <array>

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

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;

	// Get the binding description, which describes the rate to load data from memory
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	// Get the attribute descriptions, which describe how to handle vertex input
	static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};
		
		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		// Texture coordinate
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

		return attributeDescriptions;
	}
};

struct UniformBufferObject {
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
	VkQueue presentQueue;
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	std::vector<VkFramebuffer> swapChainFramebuffers;
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	VkBuffer uniformBuffer;
	VkDeviceMemory uniformBufferMemory;
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;
	
	// Initialize the GLFW window
	void initWindow();
	
	// Window resize callback
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

	// Create the descriptor set layout, which describes the details about every descriptor binding used in shaders
	void createDescriptorSetLayout();

	// Create the graphics pipeline
	void createGraphicsPipeline();

	// Create the framebuffers, one for each image in the swap chain
	void createFramebuffers();

	// Create a command pool, which manages the memory that is used to store buffers and command buffers
	void createCommandPool();

	// Create a buffer
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

	// Create a vertex buffer
	void createVertexBuffer();

	// Create an index buffer
	void createIndexBuffer();

	// Create a uniform buffer
	void createUniformBuffer();

	// Create descriptor pool, in which descriptor sets can be allocated
	void createDescriptorPool();

	// Create the descriptor set
	void createDescriptorSet();

	// Update the uniform buffer
	void updateUniformBuffer();

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

