#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "rendyUtils.h"
#include "surface.h"
#include <windows.h>
#include <iostream>

class Pixel : public Vec3 {
	private:
		int _vpI;
		int _vpJ;

		void color(const Surface& sceneObjects, const Ray& r) {
			// If there is an intersection of this ray, color the pixel
			// based on the normal of this intersection
			Intersection sect;
			Vec3 color;
			if (sceneObjects.intersect(r, Interval(0, infinity), sect)) {
				color = (sect.normal + Vec3(1, 1, 1)) * 0.5;
			}
			else {
				// If there is no collision, color the pixel along a blue->white gradient based on the y direction
				float scalar = 0.5 * (unit(r.direction()).y() + 1.0);
				color = (Vec3(1.0, 1.0, 1.0) * (1.0 - scalar)) + (Vec3(0.5, 0.7, 1.0) * scalar);
			}

			this->x(color.x());
			this->y(color.y());
			this->z(color.z());
		}
	public:
		/*
			Color the pixel that the ray hits in the viewport.

			const Surface& sceneObjects: a pointer to a list of objects in the scene
			const Ray& r: the ray coming from the camera to the viewport
		*/
		Pixel(const Surface& sceneObjects, const Ray& r, int vpI, int vpJ) {
			_vpI = vpI;
			_vpJ = vpJ;
			color(sceneObjects, r);
		}

		Pixel(float r, float g, float b, int vpI, int vpJ) {
			this->x(r);
			this->y(g);
			this->z(b);
			_vpI = vpI;
			_vpJ = vpJ;
		}

		Pixel(Vec3 vector, int vpI, int vpJ) {
			this->x(vector.x());
			this->y(vector.y());
			this->z(vector.z());
			_vpI = vpI;
			_vpJ = vpJ;
		}

		// Setters and Getters
		const float r() const { return static_cast<int>(255.999 * this->x()); }
		void r(float r) { this->x(r); }

		const float g() const { return static_cast<int>(255.999 * this->y()); }
		void g(float g) { this->y(g); }

		const float b() const { return static_cast<int>(255.999 * this->z()); }
		void b(float b) { this->z(b); }

		const int vpI() const { return _vpI; }
		void vpI(int vpI) { _vpI = vpI; }

		const int vpJ() const { return _vpJ; }
		void vpJ(int vpJ) { _vpJ = vpJ; }

		// Get the Windows COLORREF from the RGB triplet
		COLORREF getColorRef() {
			return RGB(this->r(), this->g(), this->b());
		}
};

#endif