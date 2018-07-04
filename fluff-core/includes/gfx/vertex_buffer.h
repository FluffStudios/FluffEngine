#pragma once

#include <common.h>
#include <vector>
#include <gfx/vertex_layout.h>
#include <set>
#include <memory>

namespace fluff { namespace gfx {

	class VertexBuffer
	{
	public:
		/*
			Creates a new Vertex Buffer
		 */
		FLUFF_API VertexBuffer();

		FLUFF_API ~VertexBuffer();
		
		/*
			Binds buffer and enables attribute pointers
		 */
		void FLUFF_API Bind();
		
		/*
			Unbinds buffer and disabled attribute pointers
		 */
		void FLUFF_API Unbind();

		/*
			Releases resources held by the vertex buffer
		 */
		void FLUFF_API Release();

		/*
			Loads data to a vertex buffer
			
			Data - Float array of data
			Count - Count of floats in data array
			Layout - Layout to set buffer
		 */
		void FLUFF_API LoadToBuffer(float* Data, size_t Count, VertexLayout Layout);

		/*
			Loads data to a vertex buffer
			
			Data - Unsigned int array of data
			Count - Count of unsigned ints in data array
			Layout - Layout to set buffer
		 */
		void FLUFF_API LoadToBuffer(unsigned int* Data, size_t Count, VertexLayout Layout);

		/*
			Loads data to a vertex buffer
			
			Data - Int array of data
			Count - Count of ints in data array
			Layout - Layout to set buffer
		 */
		void FLUFF_API LoadToBuffer(int* Data, size_t Count, VertexLayout Layout);

		/*
			Loads data to a vertex buffer
			
			Positions - Float array of data representing positions
			VertexData - Float array of data representing interleaved vertex attributes
		 */
		void FLUFF_API LoadToBuffer(std::vector<float> Positions, std::vector<float> VertexData);

		/*
			Loads data to a index buffer
			
			Data - Unsigned int array of data
			Count - Count of unsigned ints in data array
		 */
		void FLUFF_API LoadIndices(unsigned int* Data, size_t Count);

		/*
			Checks if two vertex buffers are the same on the GPU

			Other - other vertex buffer
			Returns if equivalent
		*/
		bool FLUFF_API operator == (const VertexBuffer & Other);

		/*
			Gets ID of vertex buffer
		*/
		unsigned int FLUFF_API GetID() const;
	private:
		struct VertexBufferImpl;
		std::unique_ptr<VertexBufferImpl> Buffer_;
	};

} }
