#pragma once

#include "math_utils.h"

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

	[[nodiscard]] bool near_zero() const noexcept {
		// Return true if the vector is close to zero in all dimensions.
		constexpr auto s = 1e-8;
		return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
	}

	void normalize() noexcept {
		*this /= length();
	}

	static Vec3 random() noexcept {
		return Vec3{ random_double(), random_double(), random_double() };
	}

	static Vec3 random(double min, double max) noexcept {
		return Vec3{ random_double(min, max), random_double(min, max), random_double(min, max) };
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

[[nodiscard]] inline Vec3 unit_vector(Vec3 v) noexcept {
	return v /= std::sqrt(v.length_squared());
}

[[nodiscard]] inline Vec3 random_unit_vector() noexcept {
	while (true) {
		auto p = Vec3::random(-1, 1);
		auto lensq = p.length_squared();
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrt(lensq);

	}
}

[[nodiscard]] inline Vec3 random_on_hemisphere(const Vec3& normal) noexcept {
	Vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}

[[nodiscard]] inline Vec3 random_in_unit_disk() noexcept {
	while (true) {
		auto p = Vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() < 1.0)
			return p;
	}
}

[[nodiscard]] inline Vec3 reflect(const Vec3& v, const Vec3& n) noexcept {
	return v - 2 * dot(v, n) * n;
}

[[nodiscard]] inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) noexcept {
	auto cos_theta = std::fmin(dot(-uv, n), 1.0);
	Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}