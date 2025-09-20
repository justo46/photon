#pragma once

#include "Hittable.h"
#include "Color.h"

#include <vector>

class Camera {
public:
	Camera(int image_width, double aspect_ratio, int samples_per_pixel, int max_depth, double vfov,
		   Point3 lookfrom, Point3 lookat, Vec3 vup, double defocus_angle, double focus_dist) noexcept;

	std::vector<uint8_t> render(const Hittable& world) noexcept;

private:
	[[nodiscard]] Color rayColor(const Ray& r, int depth, const Hittable& world) const noexcept;
	[[nodiscard]] Ray getRay(int i, int j) const noexcept;
	[[nodiscard]] Vec3 sample_square() const noexcept;
	[[nodiscard]] Point3 defocus_disk_sample() const noexcept;

public:
	double aspect_ratio = 16.0 / 9.0;	// Ratio of image width over height
	int image_width = 1280;				// Rendered image width in pixel count
	int samples_per_pixel = 10;			// Number of samples per pixel for antialiasing
	int max_depth = 10;					// Maximum ray bounce depth

	double vfov = 90.0;					// Vertical field of view in degrees
	Point3 lookfrom = Point3(0, 0, 0);	// Camera position
	Point3 lookat = Point3(0, 0, -1);	// Point to look at
	Vec3   vup = Vec3(0, 1, 0);			// "Up" direction

	double defocus_angle = 0.0;			// Variation angle of rays through each pixel
	double focus_dist = 10.0;			// Distance from camera lookfrom point to plane of perfect focus

private:
	int    image_height;	// Rendered image height
	double inv_pixel_samples;
	Point3 center;			// Camera center
	Point3 pixel00_loc;		// Location of pixel 0, 0
	Vec3   pixel_delta_u;	// Offset to pixel to the right
	Vec3   pixel_delta_v;	// Offset to pixel below
	Vec3   u, v, w;			// Camera frame basis vectors
	Vec3   defocus_disk_u, defocus_disk_v;
};
