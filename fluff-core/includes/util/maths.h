#pragma once

#include <common.h>
#include <complex>
#include <vector>

namespace fluff
{

	/*
		Creates a vector of white noise

		Rows - number of rows
		Columns - number of columns
		Seed - seed
		Returns white noise
	*/
	FLUFF_API std::vector<float> GenerateWhiteNoise(size_t Rows, size_t Columns, size_t Seed);
	
	/*
		Creates a vector of white noise with offsets

		Rows - number of rows
		Columns - number of columns
		X - beginning X value
		Z - beginning Z value
		Seed - seed
		Returns white noise
	*/
	FLUFF_API std::vector<float> GenerateWhiteNoise(size_t Rows, size_t Columns, size_t X, size_t Z, size_t Seed);

	/*
		Performs an FFT on a dataset

		Width - Number of Columns
		Height - Number of Rows
		Returns data in frequency domain
	*/
	FLUFF_API std::vector<std::complex<float>> FFT(std::vector<std::complex<float>> InputData, uint32_t Width, uint32_t Height);
	
	/*
		Performs transformation/filter on data in the frequency domain

		Width - Number of Columns
		Height - Number of Rows
		TransformationFilter - Value to filter out
		Returns filtered data
	*/
	FLUFF_API std::vector<std::complex<float>> Transform(std::vector<std::complex<float>> FFTData, uint32_t Width, uint32_t Height, float32_t TransformationFilter);
	
	/*
		Performs an inverse FFT on a dataset

		FFTData - Data to perform IFFT on
		Width - Number of columns
		Height - Number of rows
		Returns data in spacial domain
	*/
	FLUFF_API std::vector<std::complex<float>> IFFT(std::vector<std::complex<float>> FFTData, uint32_t Width, uint32_t Height);
	
	/*
		Normalizes data from [0, 1]

		Input - Data to normalize
		Lower - Lowest point
		Upper - Largest point
		Returns normalized ata
	*/
	FLUFF_API std::vector<float> Normalize(std::vector<float> Input, float32_t Lower, float32_t Upper);

	/*
		Performs a cosine interpolation between two data points

		Left - lower bound
		Right - upper bound
		Blend - factor of blending on a cosine wave
		Returns blended value
	*/
	FLUFF_API float32_t CosineInterpolation(float32_t Left, float32_t Right, float32_t Blend);

}