#pragma once

#include <core/ecs/entity.h>

#include <physics/physics_actor.h>
#include <physics/physics_material.h>
#include <physics/physics_geometry.h>
#include <physics/physics_system.h>

#include <core/transformation_component.h>

#include <core/timer.h>

namespace fluff { namespace physics {

	class PhysicsComponent : public ecs::Component<PhysicsComponent>
	{
		PhysicsActor * pActor_;
		PhysicsMaterial * pMaterial_;
	public:
		PhysicsComponent(ActorType Actor, GeometryType Geometry, GeometryDesc * GeomDesc, TransformationComponent * Transform, PhysicsMaterialDescriptor Material, PhysicsSystem * System, float Density = 0);
		~PhysicsComponent();

		PhysicsActor * GetActor() const { return pActor_; }
	};

} }