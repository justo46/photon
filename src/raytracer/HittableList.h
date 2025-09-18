#pragma once

#include "Hittable.h"
#include "math_utils.h"

#include <vector>

class HittableList : public Hittable {
public:
	std::vector<std::shared_ptr<Hittable>> objects;

	HittableList() noexcept = default;
	HittableList(std::shared_ptr<Hittable> object) noexcept {
		add(object);
	}

	void clear() noexcept { objects.clear(); }

	void add(std::shared_ptr<Hittable> object) noexcept { objects.push_back(object); }

	bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const noexcept override {
		HitRecord tempRec;
		bool hitAnything = false;
		double closestSoFar = ray_t.max;
		for (const auto& object : objects) {
			if (object->hit(r, Interval(ray_t.min, closestSoFar), tempRec)) {
				hitAnything = true;
				closestSoFar = tempRec.t;
				rec = tempRec;
			}
		}
		return hitAnything;
	}
};