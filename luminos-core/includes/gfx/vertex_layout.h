#pragma once

namespace luminos { namespace gfx {

	LUMINOS_API struct VertexLayout
	{
		unsigned int Attribute;
		unsigned int Dimensions;
		size_t Offset;
		size_t Stride;
	};

} }