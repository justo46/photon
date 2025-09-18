#pragma once

#include "math_utils.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Camera.h"

std::vector<uint8_t> raytrace() {
	HittableList world;

	world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
	world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

	Camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 1280;
	return cam.render(world);
}