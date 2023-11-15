#pragma once
#ifndef RAY_H
#define RAY_H

#include "vec3.h"
#include "color.h"

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

		// Set a blue gradient based on the y direction of the ray
		Color color() const {
			float scalar = 0.5 * (unit(dir).y() + 1.0);
			return Color(
				(Vec3(1.0, 1.0, 1.0) * (1.0 - scalar)) + (Vec3(0.5, 0.7, 1.0) * scalar)
			);
		}
};

#endif
