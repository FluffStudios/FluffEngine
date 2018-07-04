#pragma once

#include <gfx/shader_buffers/components.h>
#include <gfx/shader_buffers/lighting.h>

#define MAX_LIGHTS_SCENE 32

namespace fluff { namespace render {

	struct SceneLighting
	{
		gfx::IVec3 LightCounts; // DirectionalLight count should be 0, 1
		gfx::DirectionalLight D_Light;
		gfx::PointLight P_Lights[MAX_LIGHTS_SCENE];
		gfx::SpotLight	S_Lights[MAX_LIGHTS_SCENE];
	};

} }