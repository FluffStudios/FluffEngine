#pragma once

#include <core/transformation.h>
#include <rendering/renderable.h>
#include <util/FastNoise.h>

namespace fluff { namespace render
{
	
	class FLUFF_API Terrain : public Renderable
	{
		float * HeightMap_;
		size_t VertexCountPerSide_;
	public:
		/*
			Default constructor
		*/
		Terrain() { }

		/*
			Creates new terrain
			
			VertexCountPerSide - vertices per side
		 */
		explicit Terrain(unsigned int VertexCountPerSide);

		Terrain(const Terrain & Other);

		~Terrain()
		{
			if (HeightMap_)
				delete HeightMap_;
		}

		/*
			Creates new terrain
			
			VertexCountPerSide - vertices per side
			MaterialHandle - pointer to material
		 */
		Terrain(unsigned int VertexCountPerSide, Material *MaterialHandle);

		/*
			Creates new terrain
			
			VertexCountPerSide - vertices per side
			MaterialHandle - pointer to material
			ModelHandle	- pointer to model
		 */
		Terrain(unsigned int VertexCountPerSide, Material *MaterialHandle, Model *ModelHandle);

		Terrain(unsigned int VertexCountPerSide, Material * MaterialHandle, Transformation * TransformHandle, FastNoise & Noise, const float Amplitude, const unsigned int Size);

		/*
			Generates a flat terrain

			VertexCountPerSide - Number of vertices on each side of terrain
			MaterialHandle - Material to use in rendering
			TransformHandle - Handle for transformation
			Returns terrain
		*/
		static Terrain* GenerateFlatTerrain(unsigned int VertexCountPerSide, Material *MaterialHandle, Transformation *TransformHandle);

		/*
			Generates terrain from height map

			VertexCountPerSide - Number of vertices on each side of terrain
			MaterialHandle - Material to use in rendering
			TransformHandle - Handle for transformation
			HeightMap - Map to calculate height values with
			Amplitude - Multiplier for heights
			Returns terrain
		*/
		static Terrain* GenerateTerrainFromMap(unsigned int VertexCountPerSide, Material *MaterialHandle, Transformation *TransformHandle, std::vector<float> HeightMap, float32_t Amplitude);

		/*
			Generates terrain from noise

			VertexCountPerSide - Number of vertices on each side of terrain
			MaterialHandle - Material to use in rendering
			TransformHandle - Handle for transformation
			Noise - FastNoise function
			Amplitude - Multiplier for heights
			Size - Size of Terrain
			Returns terrain
		*/
		static Terrain* GenerateTerrainFromNoiseFunction(unsigned int VertexCountPerSide, Material *MaterialHandle, Transformation *TransformHandle, FastNoise & Noise, float32_t Amplitude, const unsigned int Size);
		
		/*
			Generates terrain from noise

			VertexCountPerSide - Number of vertices on each side of terrain
			Samples - How many times noise should be sampled
			MaterialHandle - Material to use in rendering
			TransformHandle - Handle for transformation
			Noise - FastNoise function
			Amplitude - Multiplier for heights
			Size - Size of Terrain
			Returns terrain
		*/
		static Terrain* GenerateTerrainFromNoiseFunction(unsigned int VertexCountPerSide, unsigned int Samples, Material *MaterialHandle, Transformation *TransformHandle, FastNoise & Noise, float32_t Amplitude, const unsigned int Size);
	
		/*
			Converts positions, normals, texture coordinates, and indices to a terrain

			Positions - position of vertices
			Normals - normal of vertices
			UVCoordinate - uv coordinate of vertices
			Indices - Position ordering
			Mat - Material for rendering terrain
		*/
		static Terrain* GenerateTerrain(std::vector<float32_t> Positions, std::vector<float32_t> Normals, std::vector<float32_t> UVCoordinate, std::vector<uint32_t> Indices, Material * Mat = nullptr);
	
		float * GetHeightMap() const
		{
			return HeightMap_;
		}
	private:
		static float Smooth(float BL, float BR, float TL, float TR, float X, float Z);
	};

} }
