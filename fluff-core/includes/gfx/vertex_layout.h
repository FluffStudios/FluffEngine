#pragma once

#include <common.h>

namespace fluff { namespace gfx {

	struct VertexLayout
	{
		unsigned int Attribute;
		unsigned int Dimensions;
		size_t Offset;
		size_t Stride;
	};

} }