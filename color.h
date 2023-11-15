#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

class Color : public Vec3 {
	public:
		Color(float r, float g, float b) {
			this->x(r);
			this->y(g);
			this->z(b);
		}

		Color(Vec3 vector) {
			this->x(vector.x());
			this->y(vector.y());
			this->z(vector.z());
		}

		// Setters and Getters
		const float r() const { return static_cast<int>(255.999 * this->x()); }
		void r(float r) { this->x(r); }

		const float g() const { return static_cast<int>(255.999 * this->y()); }
		void g(float g) { this->g(g); }

		const float b() const { return static_cast<int>(255.999 * this->z()); }
		void b(float b) { this->b(b); }

		// Writes color value to stream passed in
		void writeColor(std::ostream& out) {
			out << static_cast<int>(255.999 * this->x()) << ' '
				<< static_cast<int>(255.999 * this->y()) << ' '
				<< static_cast<int>(255.999 * this->z()) << std::endl;
		}
};

#endif