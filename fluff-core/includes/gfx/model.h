#pragma once

#include <gfx/vertex_buffer.h>

namespace fluff { namespace gfx {

	struct FLUFF_API Model
	{
		VertexBuffer * Buffer;
		size_t IndexCount;
		size_t IndexOffset;
		size_t VertexOffset;

		/*
			Draws the model to the screen
		*/
		void Draw() const;
	};

} }
