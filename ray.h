#pragma once
#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
	private:
		Vec3 orig;
		Vec3 dir;

	public:
		Ray() {}
		Ray(const Vec3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

		// Getters
		Vec3 origin() const { return orig; }
		Vec3 direction() const { return dir; }
		Vec3 at(float t) const { return orig + (dir*t); }
};

#endif
