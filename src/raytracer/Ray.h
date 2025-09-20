#pragma once

#include "vec3.h"

class Ray {
public:
	Ray() noexcept = default;

	Ray(const Point3& origin, const Vec3& direction) noexcept
		: orig(origin), dir(direction) {}

	[[nodiscard]] constexpr const Point3& origin() const noexcept { return orig; }
	[[nodiscard]] constexpr const Vec3& direction() const noexcept { return dir; }

	[[nodiscard]] constexpr Point3 at(double t) const noexcept { 
		return orig + t * dir; 
	}

private:
	Point3 orig;
	Vec3 dir;
};