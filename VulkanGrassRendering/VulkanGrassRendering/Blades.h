#pragma once

#include <glm/glm.hpp>

#include <vector>

#include "Model.h"

const int NUM_BLADES = 512;

struct Blade {
	// Position and width
	glm::vec4 v0;
	// Bezier point and stiffness coefficient
	glm::vec4 v1;
	// Physical model guide and direction
	glm::vec4 v2;
	// Up vector and height
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

