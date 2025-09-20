#pragma once

#include "../Hittable.h"
#include "../Color.h"

class Material {

public:
	virtual ~Material() = default;

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const noexcept = 0;
};