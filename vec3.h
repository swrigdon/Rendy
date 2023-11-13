#pragma once
#ifndef Vec3_H
#define Vec3_H

#include <cmath>
#include <iostream>

class Vec3 {
	private:
		float e[3];
	public:
		// Constructors
		Vec3() : e{ 0,0,0 } {}
		Vec3(float e0, float e1, float e2) : e{ e0, e1, e2 } {}

		// Getters and Setters
		const float x() const { return e[0]; }
		void x(float x) { e[0] = x; }

		const float y() const { return e[1]; }
		void y(float y) { e[1] = y; }

		const float z() const { return e[2]; }
		void z(float z) { e[2] = z; }

		// Operator Functions
		Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
		float operator[](int i) const { return e[i]; }
		float& operator[](int i) { return e[i]; }
		Vec3& operator+=(const Vec3& v) {
			e[0] += v.e[0];
			e[1] += v.e[1];
			e[2] += v.e[2];

			return *this;
		}
		Vec3& operator*=(const Vec3& v) {
			e[0] *= v.e[0];
			e[1] *= v.e[1];
			e[2] *= v.e[2];

			return *this;
		}
		Vec3& operator/=(const Vec3& v) {
			e[0] /= v.e[0];
			e[1] /= v.e[1];
			e[2] /= v.e[2];

			return *this;
		}
		Vec3& operator-=(const Vec3& v) {
			e[0] -= v.e[0];
			e[1] -= v.e[1];
			e[2] -= v.e[2];

			return *this;
		}

		// Length Helpers
		float lengthSquared() const {
			return (e[0] * e[0]) + (e[1] * e[1]) + (e[2] * e[2]);
		}
		float length() const {
			return std::sqrt(lengthSquared());
		}
};

// Vector Operator Helper Functions
inline std::ostream &operator <<(std::ostream &out, const Vec3 &v) {
	return out << v.x() << ' ' << v.y() << ' ' << v.z();
}
inline Vec3 operator+(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}
inline Vec3 operator-(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}
inline Vec3 operator*(const Vec3 &u, const Vec3 &v) {
	return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}
inline Vec3 operator*(const Vec3 &u, float t) {
	return Vec3(u.x() * t, u.y() * t, u.z() * t);
}
inline Vec3 operator/(const Vec3 &u, float t) {
	return Vec3(u.x() / t, u.y() / t, u.z() / t);
}

// Vector Math Helper Functions
inline float dot(const Vec3 &u, const Vec3 &v) {
	return u.x() * v.x()
		+ u.y() * v.y()
		+ u.z() * v.z();
}
inline Vec3 cross(const Vec3 &u, const Vec3 &v) {
	return Vec3(
		u.y() * v.z() - u.z() * v.y(),
		u.z() * v.x() - u.x() * v.z(),
		u.x() * v.y() - u.y() * v.x()
	);
}
inline Vec3 unit(Vec3 u) {
	return u / u.length();
}

#endif
