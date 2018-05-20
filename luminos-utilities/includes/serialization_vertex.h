#pragma once

#include "common.h"

namespace luminos { namespace utilities
{
	
	struct LUMINOS_API VertexSerial
	{
		float Position[3];
		float Uv[3];
		float Normal[3];
		float Tangent[3];
		float Bitangent[3];
		float Diffuse[4];
		float Specular[4];
	};

} }