#pragma once

#define STB_IMAGE_IMPLEMENTATION    

#include <stb_image.h>

namespace fluff
{
	/*
		Reads image from file
		
		Filename - File to read from
		Width - Reference to get width of image
		Height - Reference to get height of image
		BytesPerPixel - Reference to the number of bytes per pixel
		Flip - Determines whether to flip the image vertically
		Channels - Number of channels to load
		Returns array of pixels
	 */
	static unsigned char* read_image_file(const char* Filename, int& Width, int& Height, int& BytesPerPixel, bool Flip, unsigned int Channels)
	{
		stbi_set_flip_vertically_on_load(Flip);
		return stbi_load(Filename, &Width, &Height, &BytesPerPixel, Channels);
	}

	/*
		Frees an image from memory
		
		Data - pointer to block of pixels
	 */
	static void free_image_file(unsigned char* Data)
	{
		stbi_image_free(Data);
		Data = nullptr;
	}
}