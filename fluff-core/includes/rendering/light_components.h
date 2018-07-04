#pragma once

#include <core/ecs/entity.h>
#include <gfx/shader_buffers/lighting.h>

namespace fluff { namespace render {

	struct DirectionalLightComponent : public ecs::Component<DirectionalLightComponent>
	{
		gfx::DirectionalLight Light;

		DirectionalLightComponent() { }
		DirectionalLightComponent(gfx::DirectionalLight Light) : Light(Light) { }
	};

	struct PointLightComponent : public ecs::Component<PointLightComponent>
	{
		gfx::PointLight Light;

		PointLightComponent() { }
		PointLightComponent(gfx::PointLight Light) : Light(Light) { }
	};

	struct SpotLightComponent : public ecs::Component<SpotLightComponent>
	{
		gfx::SpotLight Light;

		SpotLightComponent() { }
		SpotLightComponent(gfx::SpotLight Light) : Light(Light) { }
	};

} }