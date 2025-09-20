#pragma once

#include "Hittable.h"
#include "Vec3.h"

class Sphere : public Hittable {
public:
	Sphere(const Point3& cen, double r, std::shared_ptr<Material> mat) noexcept : center(cen), radius(std::fmax(0,r)), mat(mat) {}

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
		Vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.mat = mat;

		return true;
	}

private:
	Point3 center;
	double radius;
	std::shared_ptr<Material> mat;
};