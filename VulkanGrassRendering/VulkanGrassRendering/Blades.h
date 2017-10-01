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
};

class Blades {
private:
	std::vector<Blade> blades;

public:
	Blades();
	Blades(Model *model, int numberOfBlades);

	Blade* data();
};

