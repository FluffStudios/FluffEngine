#pragma once

namespace fluff {	namespace physics {

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
		PhysicsMaterial(void * SDK, PhysicsMaterialDescriptor Desc);
		~PhysicsMaterial();
		void * GetPhysxMaterial() const;
	};

} }