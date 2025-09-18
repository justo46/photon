#include "Camera.h"

std::vector<uint8_t> Camera::render(const Hittable& world) noexcept
{
	initialize();

	std::vector<uint8_t> rgba(image_width * image_height * 4);

	for (int j = 0; j < image_height; ++j) {
		for (int i = 0; i < image_width; ++i) {
			const auto u = double(i);
			const auto v = double(j);
			const Ray r(center, pixel00_loc + u * pixel_delta_u + v * pixel_delta_v - center);
			const Color pixel_color = rayColor(r, world, 0);

			rgba[4 * (j * image_width + i) + 0] = static_cast<uint8_t>(255.999 * pixel_color.x());
			rgba[4 * (j * image_width + i) + 1] = static_cast<uint8_t>(255.999 * pixel_color.y());
			rgba[4 * (j * image_width + i) + 2] = static_cast<uint8_t>(255.999 * pixel_color.z());
			rgba[4 * (j * image_width + i) + 3] = 255;
		}
	}

	return rgba;
}

void Camera::initialize() noexcept
{
	image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = image_height < 1.0 ? 1.0 : image_height;

	center = Point3(0, 0, 0);

	// Determine viewport dimensions.
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewport_u = Vec3(viewport_width, 0, 0);
	auto viewport_v = Vec3(0, -viewport_height, 0);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	// Calculate the location of the upper left pixel.
	auto viewport_upper_left =
		center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
	pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

}

Color Camera::rayColor(const Ray& r, const Hittable& world, int depth) const noexcept
{
	HitRecord rec;

	if(world.hit(r, Interval(0, infinity), rec)) {
		return 0.5 * (rec.normal + Color(1, 1, 1));
	}

	Vec3 unitDirection = unit_vector(r.direction());
	auto t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}


