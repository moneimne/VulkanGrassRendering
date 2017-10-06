#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "Model.h"

const int NUM_BLADES = 512;
const float MIN_HEIGHT = 1.3f;
const float MAX_HEIGHT = 2.5f;
const float MIN_WIDTH = 0.1f;
const float MAX_WIDTH = 0.14f;
const float MIN_BEND = 0.5f;
const float MAX_BEND = 0.7f;

struct Blade {
	// Position and direction
	glm::vec4 v0;
	// Bezier point and height
	glm::vec4 v1;
	// Physical model guide and width
	glm::vec4 v2;
	// Up vector and stiffness coefficient
	glm::vec4 up;

	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Blade);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {
		std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};

		// v0
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Blade, v0);

		// v1
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Blade, v1);

		// v2
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Blade, v2);

		// up
		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32B32A32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Blade, up);

		return attributeDescriptions;
	}
};

class Blades {
private:
	std::vector<Blade> blades;

public:
	Blades();
	Blades(Model *model, int numberOfBlades);

	Blade* data();
};

