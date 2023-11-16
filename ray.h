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

			Vec3 center = Vec3(0, 0, -1);
			float t = hitObject(Vec3(0,0,-1), 0.5);
			if (t > 0.0) {
				Vec3 normal = unit(at(t) - Vec3(0, 0, -1));
				return Color(normal.x() + 1, normal.y() + 1, normal.z() + 1) * 0.5; 
			}

			float scalar = 0.5 * (unit(dir).y() + 1.0);
			return Color(
				(Vec3(1.0, 1.0, 1.0) * (1.0 - scalar)) + (Vec3(0.5, 0.7, 1.0) * scalar)
			);
		}

		float hitObject(const Vec3& center, double radius) const {
			Vec3 originCenter = orig - center;
			float a = dot(dir, dir);
			float b = 2.0 * dot(originCenter, dir);
			float c = dot(originCenter, originCenter) - (radius * radius);
			float discriminant = (b * b) - (4.0 * a * c);
			if (discriminant < 0.0) {
				return -1.0;
			}
			else {
				return (-b - sqrt(discriminant)) / (2.0 * a);
			}
		}
};

#endif
