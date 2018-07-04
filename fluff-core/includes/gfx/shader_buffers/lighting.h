#pragma once

#include <gfx/shader_buffers/components.h>

namespace fluff { namespace gfx {

	struct DirectionalLight
	{
		Vec3 Direction;
		Vec3 Ambient;
		Vec3 Diffuse;
		Vec3 Specular;
	};

	struct PointLight
	{
		Vec3 Position;
		Vec3 Ambient;
		Vec3 Diffuse;
		Vec3 Specular;
		Vec3 Attenuation;
		float buffer[12];
	};

	struct SpotLight
	{
		Vec3 Position;
		Vec3 Direction;
		Vec3 Ambient;
		Vec3 Diffuse;
		Vec3 Specular;
		Vec3 Attenuation;
		Float Cutoff;
		float buffer[7];
	};

} }