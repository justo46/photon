#pragma once

#include <iostream>
#include <vector>

#include "Vec3.h"

using Color = Vec3;

void write_color(std::vector<uint8_t>& colorBuffer, const Color& pixel_color, int i, int j, int image_width);
