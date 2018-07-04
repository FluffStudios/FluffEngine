#pragma once

#include <common.h>

namespace fluff {namespace physics {

	struct PhysicsMaterialDescriptor
	{
		float DynamicFriction;
		float StaticFriction;
		float Restitution;
	};

	class PhysicsMaterial
	{
		void * Material_;
	public:
		FLUFF_API PhysicsMaterial(void * SDK, PhysicsMaterialDescriptor Desc);
		FLUFF_API ~PhysicsMaterial();
		void FLUFF_API * GetPhysxMaterial() const;
	};

} }