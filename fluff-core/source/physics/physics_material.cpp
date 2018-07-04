#include <physics/physics_material.h>

#include <Physx/PxMaterial.h>
#include <Physx/PxPhysicsAPI.h>

namespace fluff { namespace physics { 

	PhysicsMaterial::PhysicsMaterial(void * SDK, PhysicsMaterialDescriptor Desc)
	{
		Material_ = ((physx::PxPhysics *) SDK)->createMaterial(Desc.StaticFriction, Desc.DynamicFriction, Desc.Restitution);
	}

	PhysicsMaterial::~PhysicsMaterial()
	{
		if (((physx::PxMaterial *) Material_)->isReleasable())
			((physx::PxMaterial *) Material_)->release();
	}

	void* PhysicsMaterial::GetPhysxMaterial() const
	{
		return Material_;
	}

} }