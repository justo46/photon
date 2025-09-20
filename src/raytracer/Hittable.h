#pragma once

#include "Ray.h"
#include "Interval.h"
#include "Vec3.h"

class Material;

class HitRecord {
public:
	Point3 p;
	Vec3 normal;
	std::shared_ptr<Material> mat;
	double t{0.0};
	bool front_face{false};

	void set_face_normal(const Ray& r, const Vec3& outward_normal) noexcept {
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable {
public:
	virtual ~Hittable() = default;
	virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const noexcept = 0;
};