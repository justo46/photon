#pragma once

#include <SDL3/SDL_opengl.h>
#include <vector>

#include "raytracer/Raytracer.h"

// Globals (or part of your app state)
GLuint gTex = 0;

// Call this once AFTER creating the GL context + ImGui, BEFORE the main loop
void InitRaytracedImageOnce(int width, int height)
{
	std::vector<uint8_t> pixels = raytrace(); // your function

	printf("W=%d H=%d size=%zu expected=%zu ptr=%p\n",
		width, height, pixels.size(), size_t(4) * width * height, pixels.data());


	glGenTextures(1, &gTex);
	glBindTexture(GL_TEXTURE_2D, gTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // or GL_LINEAR if you’ll scale
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Upload once
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}


