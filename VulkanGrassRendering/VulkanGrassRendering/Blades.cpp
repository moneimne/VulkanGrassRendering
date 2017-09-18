#include "Blades.h"

Blades::Blades() {
}

float generateRandomFloat() {
	return rand() / (float)RAND_MAX;
}

Blades::Blades(Model model, int numberOfBlades) {
	int size = numberOfBlades * sizeof(glm::vec4);

	v0 = (glm::vec4*)malloc(size);

	for (int i = 0; i < numberOfBlades; i++) {
		// Generate positions
		float x = generateRandomFloat() - 0.5f;
		float y = 0.0f;
		float z = generateRandomFloat() - 0.5f;

		v0[i] = glm::vec4(x, y, z, 0.01f);
	}
}