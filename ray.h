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

			// If there is a collision with an object, color the pixel based on the 
			// Normal vector of the array
			float t = hitObject(Vec3(0,0,-1), 0.5);
			if (t > 0.0) {
				Vec3 normal = unit(at(t) - Vec3(0, 0, -1));
				return Color(normal.x() + 1, normal.y() + 1, normal.z() + 1) * 0.5; 
			}

			// If there is no collision, color the pixel along a blue->white gradient based on the y direction
			float scalar = 0.5 * (unit(dir).y() + 1.0);
			return Color(
				(Vec3(1.0, 1.0, 1.0) * (1.0 - scalar)) + (Vec3(0.5, 0.7, 1.0) * scalar)
			);
		}

		/*	
			Calculate the collision for a sphere by calculating a discriminant
			using the offset from the center, the direction of the ray,
			and the radius of the sphere, then using that discriminant if it is zero
			or positive to calculate the quadratic formula for collisions. If the 
			quadratic result is zero or positive, there is at least one collision.

			See https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere
			for the full mathematical breakdown
		*/
		float hitObject(const Vec3& center, double radius) const {
			// Calculate the offset of origin from the center of the camera
			Vec3 originCenter = orig - center;
			// We can simplify the dot of a vector with itself to be the square of it's length
			//float a = dot(dir, dir);
			float a = dir.lengthSquared();
			/*
				Since the equation for b has a factor of 2 in it, and the quadratic equation
				divides by 2a, we can simplify. We can also pull the 2^2, or 4, out from the
				square root portion of the discriminant

				See https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects
			*/
			//float b = 2.0 * dot(originCenter, dir);
			float halfB = dot(originCenter, dir);
			// We can simplify the dot of a vector with itself to be the square of it's length
			//float c = dot(originCenter, originCenter) - (radius * radius);
			float c = originCenter.lengthSquared() - radius * radius;
			// Our quadratic discriminant formula is b^2 - 4ac. Taking out 4 we get b/2^2 - ac
			//float discriminant = (b * b) - (4.0 * a * c);
			float discriminant = (halfB * halfB) - (a * c);

			// Since we simplified and factored out 2.0 above, our quadratic is simplified as such
			//return discriminant < 0.0 ? -1.0 : (-b - sqrt(discriminant)) / (2.0 * a);
			return discriminant < 0.0 ? -1.0 : (-halfB - sqrt(discriminant)) / a;
		}
};

#endif
