#pragma once
#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <iostream>

class Color : public Vec3 {
	
	public:
		Color(float x, float y, float z) {
			this->x(x);
			this->y(y);
			this->z(z);
		}

		Color(Vec3 vector) {
			this->x(vector.x());
			this->y(vector.y());
			this->z(vector.z());
		}

		void writeColor(std::ostream& out) {
			out << static_cast<int>(255.999 * this->x()) << ' '
				<< static_cast<int>(255.999 * this->y()) << ' '
				<< static_cast<int>(255.999 * this->z()) << std::endl;
		}
};

#endif