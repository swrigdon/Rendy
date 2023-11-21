#pragma once
#ifndef RENDYUTILS_H
#define RENDYUTILS_H

#include <cmath>
#include <limits>
#include <memory>

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.1415926535897932385;

// Utility Functions
inline float radians(float degrees) {
	return degrees * pi / 180.0;
}

// Common Headers
#include "vec3.h"
#include "ray.h"
#include "interval.h"

#endif
