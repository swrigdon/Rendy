#pragma once
#ifndef SURFACE_H
#define SURCACE_H

#include "rendyUtils.h"
#include <memory>
#include <vector>


/*
	The Intersection class contains all information regarding
	where a ray intersects with a surface. 
*/
class Intersection {
	public:
		Vec3 point;
		Vec3 normal;
		// t is a scalar that scales the direction vector of a ray. We could theorectically think of
		// this scalar as time.
		float t;
		bool frontFace;

		/*
			setFaceNormal takes in a ray and the outwardNormal of that ray,
			and calculates whether the ray is pointing to the surface from within
			the object or from the outside of the object, and adjusts the normal
			for the intersection accordingly

			See: https://raytracing.github.io/books/RayTracingInOneWeekend.html#surfacenormalsandmultipleobjects/frontfacesversusbackfaces
		*/
		void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
			// If the dot product of the ray's direction and the outwardNormal is
			// positive, then the two vectors are facing the same direction and the
			// ray is inside the surface. If it is negative, then the vectors are
			// facing the opposite direction and the ray is outside the surface.
			this->frontFace = dot(r.direction(), outwardNormal) < 0;
			// If the ray is coming from outside the surface, keep the outward normal.
			// If the ray is coming from inside the surface, invert the outward normal.
			this->normal = frontFace ? outwardNormal : -outwardNormal;
		}
};


/*
	The Surface class is an abstract class that contains an intersect method that determines whether
	this surface has been hit by a ray. Can be extended by all entities in a scene.
*/
class Surface {
	public:
		virtual ~Surface() = default;

		virtual bool intersect(const Ray& r, Interval rayT, Intersection& sect) const = 0;
};

class SurfaceList : public Surface {
	public:
		/*
			std::shared_ptr is a pointer to an allocated data type that has a metadata counter
			that tracks the number of references to the pointer. The counter is decremented
			when the reference goes out of scope of execution. When that counter reaches zero,
			the object is safely deleted.

			std::vector is a generic array-like collection type, similar to ArrayList in Java
		*/
		std::vector<std::shared_ptr<Surface>> objects;

		SurfaceList() {}
		SurfaceList(std::shared_ptr<Surface> object) { add(object); }

		void clear() { objects.clear(); }

		void add(std::shared_ptr<Surface> object) { objects.push_back(object); }

		bool intersect(const Ray& r, Interval rayT, Intersection& sect) const override {
			Intersection tempSect;
			bool intersectAnything = false;
			float closest = rayT.max;

			for (const auto& object : objects) {
				if (object->intersect(r, Interval(rayT.min, closest), tempSect)) {
					intersectAnything = true;
					closest = tempSect.t;
					sect = tempSect;
				}
			}

			return intersectAnything;
		}
};

#endif