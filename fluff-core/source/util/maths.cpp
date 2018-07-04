#include <util/maths.h>

#define LUMINOS_PI acos(0) * 2

#include <valarray>
#include <complex>

namespace fluff
{
	FLUFF_API std::vector<float> GenerateWhiteNoise(size_t Rows, size_t Columns, size_t Seed)
	{
		std::vector<float> data;

		for (auto x = 0; x < Columns; x++)
		{
			for (auto y = 0; y < Rows; y++)
			{
				float random = (float) rand() / (float) RAND_MAX;
				random += 0.0001;
				data.push_back(random);
			}
		}
		return data;
	}

	FLUFF_API std::vector<float> GenerateWhiteNoise(size_t Rows, size_t Columns, size_t X, size_t Z, size_t Seed)
	{
		std::vector<float> res(Rows * Columns);
		for (auto y = 0; y < Rows; y++)
		{
			for (auto x = 0; x < Columns; x++)
			{
				srand(Seed + abs(49632 * (int) (x + X * Columns)) + abs(325176 * (int)(y + Z * Rows)));
				float random = (float) rand() / (float) RAND_MAX;
				random > 0 ? random : 0.001f;
				res[y * Columns + x] = random;
			}
		}
		return res;
	}

	void FFTHelper(std::valarray<std::complex<float>>& Data)
	{
		const size_t N = Data.size();
		if (N <= 1) return;

		std::valarray<std::complex<float>> even = Data[std::slice(0, N / 2, 2)];
		std::valarray<std::complex<float>> odd = Data[std::slice(1, N / 2, 2)];

		FFTHelper(even);
		FFTHelper(odd);

		for (size_t k = 0; k < N / 2; ++k)
		{
			std::complex<float> t = std::polar(1.0f, (float)(-2 * LUMINOS_PI * k / N)) * odd[k];
			Data[k] = even[k] + t;
			Data[k + N / 2] = even[k] - t;
		}
	}

	std::vector<std::complex<float>> FFT(std::vector<std::complex<float>> InputData, uint32_t Width, uint32_t Height)
	{
		std::vector<std::complex<float>> res;

		for (int i = 0; i < Height; i++)
		{
			std::valarray<std::complex<float>> Data(InputData.data() + i * Width, InputData.size() / Height);
			FFTHelper(Data);
			for (auto i : Data)
			{
				res.push_back(i);
			}
		}

		std::vector<std::complex<float>> tmp(Width * Height);
		for (int i = 0; i < Width * Height; i++)
		{
			size_t column = i % Width;
			size_t row = (i - column) / Height;
			size_t newPos = column * Height + row;
			tmp[newPos] = res[i];
		}
		res.clear();
		res.reserve(Width * Height);
		for (int i = 0; i < Height; i++)
		{
			std::valarray<std::complex<float>> Data(tmp.data() + i * Width, tmp.size() / Height);
			FFTHelper(Data);
			for (auto i : Data)
			{
				res.push_back(i);
			}
		}
		for (int i = 0; i < Width * Height; i++)
		{
			size_t column = i % Width;
			size_t row = (i - column) / Height;
			size_t newPos = column * Height + row;
			res[newPos] = tmp[i];
		}
		return res;
	}

	std::vector<std::complex<float>> Transform(std::vector<std::complex<float>> FFTData, uint32_t Width, uint32_t Height, float32_t TransformationFilter)
	{
		std::vector<std::complex<float>> B(Width * Height);
		for (auto z = 0; z < Height; z++)
		{
			for (auto x = 0; x < Width; x++)
			{
				uint32_t i, j;
				if ((Width / 2 < x && x < Width) && (0 <= z && z < Height / 2))
				{
					i = Width - x;
					j = z;
				}
				else if ((0 <= x && x <= Width / 2) && (Height / 2 < z && z < Height))
				{
					i = x;
					j = Height - z;
				}
				else if ((Width / 2 < x && x < Width) && (Height / 2 < z && z < Height))
				{
					i = Width - x;
					j = Height - z;
				}
				else
				{
					i = x;
					j = z;
				}
				float zeta_i = (float)i / Width;
				float zeta_j = (float)j / Height;
				float zeta = sqrt(zeta_i * zeta_i + zeta_j * zeta_j);
				zeta = zeta == 0 ? 0.0001f : zeta;
				B[z * Height + x] = (float)sqrt(1.0f / pow(zeta, TransformationFilter)) * FFTData[z * Height + x];
			}
		}
		return B;
	}

	void IFFTHelper(std::valarray<std::complex<float>>& Data)
	{
		Data = Data.apply(std::conj);
		FFTHelper(Data);
		Data = Data.apply(std::conj);
		Data /= Data.size();
	}

	std::vector<std::complex<float>> IFFT(std::vector<std::complex<float>> InputData, uint32_t Width, uint32_t Height)
	{
		std::vector<std::complex<float>> res(Width * Height);
		std::vector<std::complex<float>> tmp;

		for (int i = 0; i < Width * Height; i++)
		{
			size_t column = i % Width;
			size_t row = (i - column) / Height;
			size_t newPos = column * Height + row;
			res[newPos] = InputData[i];
		}

		for (auto i = 0; i < Height; i++)
		{
			std::valarray<std::complex<float>> Data(res.data() + (i * Width), res.size() / Height);
			IFFTHelper(Data);
			for (auto i : Data)
			{
				tmp.push_back(i);
			}
		}

		for (int i = 0; i < Width * Height; i++)
		{
			size_t column = i % Width;
			size_t row = (i - column) / Height;
			size_t newPos = column * Height + row;
			res[newPos] = tmp[i];
		}

		tmp.clear();

		for (auto i = 0; i < Height; i++)
		{
			std::valarray<std::complex<float>> Data(res.data() + (i * Width), res.size() / Height);
			IFFTHelper(Data);
			for (auto i : Data)
			{
				tmp.push_back(i);
			}
		}

		return tmp;
	}

	FLUFF_API std::vector<float> Normalize(std::vector<float> Input, float32_t Lower, float32_t Upper)
	{
		float32_t min = std::numeric_limits<float>::max();
		float32_t max = -std::numeric_limits<float>::max();
		for (const auto i : Input)
		{
			min = min < i ? min : i;
			max = max > i ? max : i;
		}

		float32_t range = max - min;
		range = range > 0 ? range : 0.0001f;
		std::vector<float> result;
		for (const auto i : Input)
		{
			auto val = i - min;
			val /= range;
			val *= (Upper - Lower);
			val += Lower;
			result.push_back(val);
		}
		return result;
	}
	
	float32_t CosineInterpolation(float32_t Left, float32_t Right, float32_t Blend)
	{
		double theta = Blend * LUMINOS_PI;
		float f = (float) (1.0f - cos(theta));
		return Left * (1.0f - f) + Right * f;
	}
}