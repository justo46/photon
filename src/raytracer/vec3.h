#pragma once

#include <cmath>
#include <iostream>

class vec3 {
public:
	double e[3];

	constexpr vec3() noexcept : e{ 0,0,0 } {}
	constexpr vec3(double x, double y, double z) noexcept : e{ x, y, z } {}

	[[nodiscard]] constexpr double x() const noexcept { return e[0]; }
	[[nodiscard]] constexpr double y() const noexcept { return e[1]; }
	[[nodiscard]] constexpr double z() const noexcept { return e[2]; }

	[[nodiscard]] constexpr double operator[](int i) const noexcept { return e[i]; }
	constexpr double& operator[](int i) noexcept { return e[i]; }

	constexpr vec3& operator+=(const vec3& v) noexcept {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	constexpr vec3& operator-=(const vec3& v) noexcept {
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	}
	constexpr vec3& operator*=(const double t) noexcept {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	constexpr vec3& operator*=(const vec3& v) noexcept {
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	}
	constexpr vec3& operator/=(const double t) noexcept {
		return *this *= (1.0 / t);
	}

	[[nodiscard]] constexpr double length_squared() const noexcept {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	// Not constexpr because of std::sqrt
	[[nodiscard]] double length() const noexcept {
		return std::sqrt(length_squared());
	}

	constexpr vec3& normalize() noexcept {
		return *this /= length();
	}
};

// Define point3 as an alias for vec3, for code clarity
using point3 = vec3;

inline constexpr vec3 operator-(const vec3& v) noexcept {
	return vec3{ -v.e[0], -v.e[1], -v.e[2] };
}

inline constexpr vec3 operator+(vec3 a, const vec3& b) noexcept {
	return a += b;
}

inline constexpr vec3 operator-(vec3 a, const vec3& b) noexcept {
	return a -= b;
}

inline constexpr vec3 operator*(vec3 a, const double t) noexcept {
	return a *= t;
}

inline constexpr vec3 operator*(const double t, vec3 a) noexcept {
	return a *= t;
}

inline constexpr vec3 operator*(vec3 a, const vec3& b) noexcept {
	return a *= b;
}

// Not constexpr because of std::fma
[[nodiscard]] inline double dot(const vec3& a, const vec3& b) noexcept {
	return std::fma(a.e[0], b.e[0], std::fma(a.e[1], b.e[1], a.e[2] * b.e[2]));
}

[[nodiscard]] inline constexpr vec3 cross(const vec3& a, const vec3& b) noexcept {
	return vec3{ a.e[1] * b.e[2] - a.e[2] * b.e[1],
				 a.e[2] * b.e[0] - a.e[0] * b.e[2],
				 a.e[0] * b.e[1] - a.e[1] * b.e[0] };
}

[[nodiscard]] inline constexpr vec3 unit_vector(vec3 v) noexcept {
	return v /= std::sqrt(v.length_squared());
}