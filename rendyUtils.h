#pragma once
#ifndef RENDYUTILS_H
#define RENDYUTILS_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions
inline float radians(float degrees) {
	return degrees * pi / 180.0;
}

// Returns a random float in the interval [0,1)
inline float random_float() {
	return rand() / (RAND_MAX + 1.0);
}

// Returns a random float in the interval [min,max)
inline float random_float(float min, float max) {
	return min + (max-min) * random_float();
}

// Common Headers
#include "vec3.h"
#include "ray.h"
#include "interval.h"

#endif
