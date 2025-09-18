#pragma once

#include "hittable.h"
#include "vec3.h"

class Sphere : public Hittable {
public:
	Sphere(Point3 cen, double r) noexcept : center(cen), radius(r) {}

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const noexcept override {
		Vec3 oc = center - r.origin();
		const auto a = r.direction().length_squared();
		const auto h = dot(oc, r.direction());
		const auto c = oc.length_squared() - radius * radius;

		const auto discriminant = h * h - a * c;

		if (discriminant < 0) [[likely]]
			return false;

		const auto sqrt_d = std::sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range.
		auto root = (h - sqrt_d) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + sqrt_d) / a;
			if (!ray_t.surrounds(root))
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		rec.normal = (rec.p - center) / radius;

		return true;
	}

private:
	Point3 center;
	double radius;
};