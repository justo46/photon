#pragma once

#include "Hittable.h"
#include "Color.h"

#include <vector>

class Camera {
public:
	Camera(int image_width, double aspect_ratio, int samples_per_pixel, int max_depth) noexcept;

	std::vector<uint8_t> render(const Hittable& world) noexcept;

private:
	[[nodiscard]] Color rayColor(const Ray& r, int depth, const Hittable& world) const noexcept;
	[[nodiscard]] Ray getRay(int i, int j) const noexcept;
	[[nodiscard]] Vec3 sample_square() const noexcept;

public:
	double aspect_ratio = 16.0 / 9.0;	// Ratio of image width over height
	int image_width = 1280;				// Rendered image width in pixel count
	int samples_per_pixel = 10;			// Number of samples per pixel for antialiasing
	int max_depth = 10;					// Maximum ray bounce depth

private:
	int    image_height;   // Rendered image height
	double inv_pixel_samples;
	Point3 center;         // Camera center
	Point3 pixel00_loc;    // Location of pixel 0, 0
	Vec3   pixel_delta_u;  // Offset to pixel to the right
	Vec3   pixel_delta_v;  // Offset to pixel below
};
