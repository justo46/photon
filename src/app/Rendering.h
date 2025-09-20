#pragma once

#include <SDL3/SDL_opengl.h>
#include <vector>

#include "raytracer/Raytracer.h"

struct ImageWithTexture {

	ImageWithTexture(int image_width, int image_height) : width(image_width), height(image_height) 
	{
		buffer.resize(image_width * image_height * 4);

		// Create a OpenGL texture identifier
		glGenTextures(1, &gl_texture);
		glBindTexture(GL_TEXTURE_2D, gl_texture);

		// Avoid row padding issues
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Allocate immutable-ish storage (or use TexImage2D once)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		// Basic sampling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

// 		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~ImageWithTexture() {
		glDeleteTextures(1, &gl_texture);
	}

	// Delete copy and move constructors and assignment operators
	ImageWithTexture(const ImageWithTexture&) = delete;
	ImageWithTexture& operator=(const ImageWithTexture&) = delete;
	ImageWithTexture(ImageWithTexture&& other) = delete;
	ImageWithTexture& operator=(ImageWithTexture&& other) = delete;

	// Atm it binds and unbinds the texture each time
	void uploadTexture()
	{
// 		glBindTexture(GL_TEXTURE_2D, gl_texture);
// 		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.data());
// 		glBindTexture(GL_TEXTURE_2D, 0);
	}


	std::vector<uint8_t> buffer;
	GLuint gl_texture = 0;

	int width;
	int height;
};



