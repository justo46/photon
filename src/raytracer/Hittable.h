#pragma once

#include "math_utils.h"

class HitRecord {
public:
	Point3 p;
	Vec3 normal;
	double t;
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const noexcept = 0;
};