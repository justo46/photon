#pragma once

#include "math_utils.h"
#include "Hittable.h"

#include <vector>

class Camera {
public:
	std::vector<uint8_t> render(const Hittable& world) noexcept;

private:
	void initialize() noexcept;
	[[nodiscard]] Color rayColor(const Ray& r, const Hittable& world, int depth) const noexcept;

public:
	double aspect_ratio = 16.0 / 9.0;  // Ratio of image width over height
	int    image_width = 1280;  // Rendered image width in pixel count

private:
	int    image_height;   // Rendered image height
	Point3 center;         // Camera center
	Point3 pixel00_loc;    // Location of pixel 0, 0
	Vec3   pixel_delta_u;  // Offset to pixel to the right
	Vec3   pixel_delta_v;  // Offset to pixel below
};
