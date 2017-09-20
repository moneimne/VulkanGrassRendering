#include "Blades.h"

Blades::Blades() {
}

float generateRandomFloat() {
	return rand() / (float)RAND_MAX;
}

Blades::Blades(Model *model, int numberOfBlades) {

	for (int i = 0; i < numberOfBlades; i++) {
		Blade currentBlade = Blade();

		// Generate positions
		float x = generateRandomFloat() - 0.5f;
		float y = 0.0f;
		float z = generateRandomFloat() - 0.5f;

		currentBlade.v0 = glm::vec4(x, y, z, 0.01f);

		// The rest...
		currentBlade.v1 = glm::vec4(0.0f);
		currentBlade.v2 = glm::vec4(0.0f);
		currentBlade.up = glm::vec4(0.0f);

		blades.push_back(currentBlade);
	}
}

Blade* Blades::data() {
	return blades.data();
}