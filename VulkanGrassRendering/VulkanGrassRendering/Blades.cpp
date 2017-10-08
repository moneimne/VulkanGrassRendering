#include "Blades.h"
#include "Utilities.h"

Blades::Blades() {
}

float generateRandomFloat() {
	return rand() / (float)RAND_MAX;
}

Blades::Blades(Model *model, int numberOfBlades) {

	for (int i = 0; i < numberOfBlades; i++) {
		Blade currentBlade = Blade();

		glm::vec3 bladeUp(0.0f, 1.0f, 0.0f);

		// Generate positions and direction (v0)
		float x = (generateRandomFloat() - 0.5f) * 4.0f;
		float y = 0.0f;
		float z = (generateRandomFloat() - 0.5f) * 4.0f;
		float direction = generateRandomFloat() * TWO_PI;
		glm::vec3 bladePosition(x, y, z);
		currentBlade.v0 = glm::vec4(bladePosition, direction);

		// Bezier point and height (v1)
		float height = MIN_HEIGHT + (generateRandomFloat() * (MAX_HEIGHT - MIN_HEIGHT));
		currentBlade.v1 = glm::vec4(bladePosition + bladeUp * height, height);

		// Physical model guide and width (v2)
		float width = MIN_WIDTH + (generateRandomFloat() * (MAX_WIDTH - MIN_WIDTH));
		currentBlade.v2 = glm::vec4(bladePosition + bladeUp * height, width);

		// Up vector and stiffness coefficient (up)
		float stiffness = MIN_BEND + (generateRandomFloat() * (MAX_BEND - MIN_BEND));
		currentBlade.up = glm::vec4(bladeUp, stiffness);

		blades.push_back(currentBlade);
	}
}

Blade* Blades::data() {
	return blades.data();
}