#include <vulkan/vulkan.h>
#include "Renderer.h"

#include <iostream>
#include <stdexcept>
#include <functional>

int main() {
	Renderer renderer;

	try {
		renderer.run();
	}
	catch (const std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}