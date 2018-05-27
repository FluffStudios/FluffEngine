#pragma once

#include <Physx/PxMaterial.h>
#include <Physx/PxPhysicsAPI.h>

namespace luminos {	namespace physics {

	struct PhysicsMaterialDescriptor
	{
		float DynamicFriction;
		float StaticFriction;
		float Restitution;
	};

	class PhysicsMaterial
	{
		physx::PxMaterial * Material_;
	public:
		PhysicsMaterial(physx::PxPhysics * SDK, PhysicsMaterialDescriptor Desc);
		~PhysicsMaterial();
		physx::PxMaterial * GetPhysxMaterial() const;
	};

} }