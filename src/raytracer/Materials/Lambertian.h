#include "Material.h"

class Lambertian : public Material {
public:
	Lambertian(const Color& albedo) : albedo(albedo) {}

	bool scatter(const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered) const noexcept override {
		auto scatter_direction = rec.normal + random_unit_vector();

		// Catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = Ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	Color albedo;
};