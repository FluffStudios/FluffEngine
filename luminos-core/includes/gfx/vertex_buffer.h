#pragma once

#include <glew.h>
#include <common.h>
#include <vector>
#include <gfx/vertex_layout.h>
#include <set>

namespace luminos { namespace gfx {

	class LUMINOS_API VertexBuffer
	{
		GLuint Vao_;
		std::set<unsigned int> Attribs_;
		std::vector<GLuint> Vbos_;
	public:
		/*
			Creates a new Vertex Buffer
		 */
		VertexBuffer();

		~VertexBuffer() { Release(); }
		
		/*
			Binds buffer and enables attribute pointers
		 */
		void Bind();
		
		/*
			Unbinds buffer and disabled attribute pointers
		 */
		void Unbind();

		/*
			Releases resources held by the vertex buffer
		 */
		void Release();

		/*
			Loads data to a vertex buffer
			
			Data - Float array of data
			Count - Count of floats in data array
			Layout - Layout to set buffer
		 */
		void LoadToBuffer(float* Data, size_t Count, VertexLayout Layout);

		/*
			Loads data to a vertex buffer
			
			Data - Unsigned int array of data
			Count - Count of unsigned ints in data array
			Layout - Layout to set buffer
		 */
		void LoadToBuffer(unsigned int* Data, size_t Count, VertexLayout Layout);

		/*
			Loads data to a vertex buffer
			
			Data - Int array of data
			Count - Count of ints in data array
			Layout - Layout to set buffer
		 */
		void LoadToBuffer(int* Data, size_t Count, VertexLayout Layout);

		/*
			Loads data to a vertex buffer
			
			Positions - Float array of data representing positions
			VertexData - Float array of data representing interleaved vertex attributes
		 */
		void LoadToBuffer(std::vector<float> Positions, std::vector<float> VertexData);

		/*
			Loads data to a index buffer
			
			Data - Unsigned int array of data
			Count - Count of unsigned ints in data array
		 */
		void LoadIndices(unsigned int* Data, size_t Count);

		/*
			Checks if two vertex buffers are the same on the GPU

			Other - other vertex buffer
			Returns if equivalent
		*/
		bool operator == (const VertexBuffer & Other)
		{
			return this->Vao_ == Other.Vao_;
		}

		/*
			Gets ID of vertex buffer
		*/
		GLuint GetID() const
		{
			return Vao_;
		}
	};

} }
