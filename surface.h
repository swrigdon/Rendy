#pragma once
#ifndef SURFACE_H
#define SURCACE_H

#include "ray.h"

class Intersection {
	public:
		Vec3 point;
		Vec3 normal;
		float t;
};

class Surface {
	public:
		virtual ~Surface() = default;

		virtual bool intersect(const Ray& r, float rayTMin, float rayTMax, Intersection& sect) const = 0;
};

#endif