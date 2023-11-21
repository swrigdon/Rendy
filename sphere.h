#pragma once
#ifndef SPHERE_H
#define SPHERE_H

#include "rendyUtils.h"
#include "surface.h"

class Sphere : public Surface {
	public:
		Sphere(Vec3 _center, double _radius): center(_center), radius(_radius) {}

		/*
			Calculate the collision for a sphere by calculating a discriminant
			using the offset from the center, the direction of the ray,
			and the radius of the sphere, then using that discriminant if it is zero
			or positive to calculate the quadratic formula for collisions. If the
			quadratic result is zero or positive, there is at least one collision.

			See https://raytracing.github.io/books/RayTracingInOneWeekend.html#addingasphere
			for the full mathematical breakdown
		*/
		bool intersect(const Ray& r, Interval rayT, Intersection& sect) const override {
			// Calculate the offset of origin from the center of the camera
			Vec3 originCenter = r.origin() - center;
			// We can simplify the dot of a vector with itself to be the square of it's length
			//float a = dot(dir, dir);
			float a = r.direction().lengthSquared();
			/*
				Since the equation for b has a factor of 2 in it, and the quadratic equation
				divides by 2a, we can simplify. We can also pull the 2^2, or 4, out from the
				square root portion of the discriminant

				See https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects
			*/
			//float b = 2.0 * dot(originCenter, dir);
			float halfB = dot(originCenter, r.direction());
			// We can simplify the dot of a vector with itself to be the square of it's length
			//float c = dot(originCenter, originCenter) - (radius * radius);
			float c = originCenter.lengthSquared() - radius * radius;
			// Our quadratic discriminant formula is b^2 - 4ac. Taking out 4 we get b/2^2 - ac
			//float discriminant = (b * b) - (4.0 * a * c);
			float discriminant = (halfB * halfB) - (a * c);
			// If our descriminant is negative, we cannot take the square root for our
			// quadratic equation. Thus there is no intersection. Return false.
			if (discriminant < 0) {
				return false;
			}
			// Capture the square root of the discriminant for our quadratic calculations below
			float sqrtD = sqrt(discriminant);
			// In this case our root is based on a simplified quadratic formula. Since a square root can have a negative
			// and a positive solution, we choose one to start with. In this case, we choose the negative first.
			float root = (-halfB - sqrtD) / a;
			// We check if our quadratic root is within the range of rayTMin < root < rayTMax
			// where rayTMin and rayTMax are a range of t (time) that we allow the intersection to count
			if (!rayT.surrounds(root)) {
				// If the negative quadratic root is not within our range, we then try the positive root
				root = (-halfB + sqrtD) / a;
				// If neither root iw within our t range, then there is no intersection and we return false
				if (!rayT.surrounds(root)) {
					return false;
				}
			}

			sect.t = root;
			sect.point = r.at(root);
			// Our outward normal is calculated by getting the offset vector
			// of our intersection point from the center, and dividing by the radius.
			Vec3 outwardNormal = (sect.point - center) / radius;
			sect.setFaceNormal(r, outwardNormal);

			return true;
		}

	private:
		Vec3 center;
		double radius;
};

#endif