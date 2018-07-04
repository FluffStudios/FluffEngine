#include <gfx/model.h>

#include <glew.h>

namespace fluff { namespace gfx {

	void gfx::Model::Draw() const
	{
		glDrawElementsBaseVertex(GL_TRIANGLES, static_cast<GLint>(IndexCount), GL_UNSIGNED_INT, reinterpret_cast<void*>(IndexOffset), static_cast<GLint>(VertexOffset));
	}

} }