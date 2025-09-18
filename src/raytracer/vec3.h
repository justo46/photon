#pragma once

#include <cmath>
#include <iostream>

class Vec3 {
public:
	double e[3];

	constexpr Vec3() noexcept : e{ 0, 0, 0 } {}
	constexpr Vec3(double x, double y, double z) noexcept : e{ x, y, z } {}

	[[nodiscard]] constexpr double x() const noexcept { return e[0]; }
	[[nodiscard]] constexpr double y() const noexcept { return e[1]; }
	[[nodiscard]] constexpr double z() const noexcept { return e[2]; }

	[[nodiscard]] constexpr double operator[](int i) const noexcept { return e[i]; }
	constexpr double& operator[](int i) noexcept { return e[i]; }

	constexpr Vec3& operator+=(const Vec3& v) noexcept {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	constexpr Vec3& operator-=(const Vec3& v) noexcept {
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	}
	constexpr Vec3& operator*=(const double t) noexcept {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	constexpr Vec3& operator*=(const Vec3& v) noexcept {
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	}
	constexpr Vec3& operator/=(const double t) noexcept {
		return *this *= (1.0 / t);
	}

	[[nodiscard]] constexpr double length_squared() const noexcept {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// Not constexpr because of std::sqrt
	[[nodiscard]] double length() const noexcept {
		return std::sqrt(length_squared());
	}

	constexpr Vec3& normalize() noexcept {
		return *this /= length();
	}
};

// Define Point3 as an alias for Vec3, for code clarity
using Point3 = Vec3;

inline constexpr Vec3 operator-(const Vec3& v) noexcept {
	return Vec3{ -v.e[0], -v.e[1], -v.e[2] };
}

inline constexpr Vec3 operator+(Vec3 a, const Vec3& b) noexcept {
	return a += b;
}

inline constexpr Vec3 operator-(Vec3 a, const Vec3& b) noexcept {
	return a -= b;
}

inline constexpr Vec3 operator*(Vec3 a, const double t) noexcept {
	return a *= t;
}

inline constexpr Vec3 operator*(const double t, Vec3 a) noexcept {
	return a *= t;
}

inline constexpr Vec3 operator*(Vec3 a, const Vec3& b) noexcept {
	return a *= b;
}

inline constexpr Vec3 operator/(Vec3 a, const double t) noexcept {
	return a /= t;
}

// Not constexpr because of std::fma
[[nodiscard]] inline double dot(const Vec3& a, const Vec3& b) noexcept {
	return std::fma(a.e[0], b.e[0], std::fma(a.e[1], b.e[1], a.e[2] * b.e[2]));
}

[[nodiscard]] inline constexpr Vec3 cross(const Vec3& a, const Vec3& b) noexcept {
	return Vec3{ a.e[1] * b.e[2] - a.e[2] * b.e[1],
				 a.e[2] * b.e[0] - a.e[0] * b.e[2],
				 a.e[0] * b.e[1] - a.e[1] * b.e[0] };
}

[[nodiscard]] inline constexpr Vec3 unit_vector(Vec3 v) noexcept {
	return v /= std::sqrt(v.length_squared());
}