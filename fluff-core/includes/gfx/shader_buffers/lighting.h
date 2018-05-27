#pragma once

#include <gfx/shader_buffers/components.h>

namespace luminos { namespace gfx {

	struct alignas(64) DirectionalLight
	{
		Vec3 Direction;
		Vec3 Ambient;
		Vec3 Diffuse;
		Vec3 Specular;
	};

	struct alignas(64) PointLight
	{
		Vec3 Position;
		Vec3 Ambient;
		Vec3 Diffuse;
		Vec3 Specular;
		Vec3 Attenuation;
	};

	struct alignas(64) SpotLight
	{
		Vec3 Position;
		Vec3 Direction;
		Vec3 Ambient;
		Vec3 Diffuse;
		Vec3 Specular;
		Vec3 Attenuation;
		Float Cutoff;
	};

} }