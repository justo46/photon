#include "Camera.h"
#include "Materials/Material.h"

Camera::Camera(int image_width, double aspect_ratio, int samples_per_pixel, int max_depth, 
	           double vfov, Point3 lookfrom, Point3 lookat, Vec3 vup, double defocus_angle, double focus_dist) noexcept
	: aspect_ratio(aspect_ratio)
	, image_width(image_width)
	, samples_per_pixel(samples_per_pixel)
	, inv_pixel_samples(1.0 / samples_per_pixel)
	, center(lookfrom)
	, max_depth(max_depth)
	, vfov(vfov)
	, lookfrom(lookfrom)
	, lookat(lookat)
	, vup(vup)
	, defocus_angle(defocus_angle)
	, focus_dist(focus_dist)
{
	// Here do the same as initialize basically but without calling that fdunction

	image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = image_height < 1 ? 1 : image_height;

	// Determine viewport dimensions.
	auto theta = degrees_to_radians(vfov);
	auto h = std::tan(theta / 2);
	auto viewport_height = 2.0 * h * focus_dist;
	auto viewport_width = viewport_height * (double(image_width) / image_height);

	// Calculate the u,v,w unit basis vectors for the camera coordinate frame.
	w = unit_vector(lookfrom - lookat);
	u = unit_vector(cross(vup, w));
	v = cross(w, u);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewport_u = viewport_width * u;
	auto viewport_v = viewport_height * -v;

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	pixel_delta_u = viewport_u / image_width;
	pixel_delta_v = viewport_v / image_height;

	// Calculate the location of the upper left pixel.
	auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
	pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	// Calculate the camera defocus disk basis vectors.
	auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
	defocus_disk_u = u * defocus_radius;
	defocus_disk_v = v * defocus_radius;
}

std::vector<uint8_t> Camera::render(const Hittable& world) noexcept
{
	std::vector<uint8_t> rgba(image_width * image_height * 4);

	for (int j = 0; j < image_height; ++j) {
		for (int i = 0; i < image_width; ++i) {
			Color pixel_color(0, 0, 0);
			for (int sample = 0; sample < samples_per_pixel; sample++) {
				Ray r = getRay(i, j);
				pixel_color += rayColor(r, max_depth, world);
			}
			write_color(rgba, pixel_color * inv_pixel_samples, i, j, image_width);
		}
	}

	return rgba;
}

Color Camera::rayColor(const Ray& r, int depth, const Hittable& world) const noexcept
{
	if (depth <= 0) 
		return Color(0, 0, 0);

	HitRecord rec;

	if(world.hit(r, Interval(0.001, infinity), rec)) {
		Ray scattered;
		Color attenuation;
		if (rec.mat->scatter(r, rec, attenuation, scattered))
			return attenuation * rayColor(scattered, depth - 1, world);
		return Color(0, 0, 0);
	}

	Vec3 unitDirection = unit_vector(r.direction());
	auto t = 0.5 * (unitDirection.y() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

Ray Camera::getRay(int i, int j) const noexcept
{
	// Construct a camera ray originating from the origin and directed at randomly sampled
	// point around the pixel location i, j.
	auto offset = sample_square();
	auto pixel_sample = pixel00_loc + (i + offset.x()) * pixel_delta_u + (j + offset.y()) * pixel_delta_v;

	auto ray_origin = (defocus_angle <= 0.0) ? center : defocus_disk_sample();
	auto ray_direction = pixel_sample - ray_origin;

	return Ray(ray_origin, ray_direction);
}

Vec3 Camera::sample_square() const noexcept
{
	// Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
	return Vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

Point3 Camera::defocus_disk_sample() const noexcept
{
	// Returns a random point in the camera defocus disk.
	auto p = random_in_unit_disk();
	return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}
