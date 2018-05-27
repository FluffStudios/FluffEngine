#pragma once

#include "mesh.h"

#include <glew.h>
#include <gfx/vertex_buffer.h>

namespace luminos { namespace gfx {

	struct LUMINOS_API Model
	{
		VertexBuffer * Buffer;
		size_t IndexCount;
		size_t IndexOffset;
		size_t VertexOffset;

		/*
			Draws the model to the screen
		*/
		inline void Draw() const
		{
			glDrawElementsBaseVertex(GL_TRIANGLES, static_cast<GLint>(IndexCount), GL_UNSIGNED_INT, reinterpret_cast<void*>(IndexOffset), static_cast<GLint>(VertexOffset));
		}
	};

} }
