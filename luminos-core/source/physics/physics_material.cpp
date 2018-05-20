#include <physics/physics_material.h>

namespace luminos { namespace physics { 

	PhysicsMaterial::PhysicsMaterial(physx::PxPhysics * SDK, PhysicsMaterialDescriptor Desc)
	{
		Material_ = SDK->createMaterial(Desc.StaticFriction, Desc.DynamicFriction, Desc.Restitution);
	}

	PhysicsMaterial::~PhysicsMaterial()
	{
		if (Material_->isReleasable()) Material_->release();
	}

	physx::PxMaterial * PhysicsMaterial::GetPhysxMaterial() const
	{
		return Material_;
	}

} }