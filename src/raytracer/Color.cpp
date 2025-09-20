#include "Color.h"

#include "Interval.h"

inline double linear_to_gamma(double linear_component) noexcept {
	if (linear_component > 0)
		return std::sqrt(linear_component);
	return 0.0;
}

void write_color(std::vector<uint8_t>& colorBuffer, const Color& pixel_color, int i, int j, int image_width)
{
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Apply gamma correction for gamma=2.0.
	r = linear_to_gamma(r);
	g = linear_to_gamma(g);
	b = linear_to_gamma(b);

	// Translate the [0,1] component values to the byte range [0,255].
	static const Interval intensity(0.000, 0.999);
	colorBuffer[4 * (j * image_width + i) + 0] = static_cast<uint8_t>(256 * intensity.clamp(r));
	colorBuffer[4 * (j * image_width + i) + 1] = static_cast<uint8_t>(256 * intensity.clamp(g));
	colorBuffer[4 * (j * image_width + i) + 2] = static_cast<uint8_t>(256 * intensity.clamp(b));
	colorBuffer[4 * (j * image_width + i) + 3] = 255;
}