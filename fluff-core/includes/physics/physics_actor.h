#pragma once

#include <physics/physics_geometry.h>
#include <physics/physics_material.h>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <common.h>

namespace fluff { namespace physics {

	enum ActorType
	{
		STATIC,
		DYNAMIC
	};

	class PhysicsActor
	{
	public:
		virtual glm::vec3 FLUFF_API GetPosition() const = 0;
		virtual glm::vec3 FLUFF_API GetRotation() const = 0;
		virtual glm::vec3 FLUFF_API GetLinearVelocity() const = 0;
		virtual glm::vec3 FLUFF_API GetAngularVelocity() const = 0;
		virtual void FLUFF_API * GetPointer() const = 0;
	};

	class PhysicsDynamicActor : public PhysicsActor
	{
		struct PhysicsDynamicActorImpl;
		std::unique_ptr<PhysicsDynamicActorImpl> Impl_;
	public:
		PhysicsDynamicActor(void * pSDK, glm::vec3 Position, glm::quat Rotation, GeometryType Geometry, GeometryDesc * GeomDesc, PhysicsMaterialDescriptor Material, float Density);
		glm::vec3 FLUFF_API GetPosition() const override;
		glm::vec3 FLUFF_API GetRotation() const override;
		glm::vec3 FLUFF_API GetLinearVelocity() const override;
		glm::vec3 FLUFF_API GetAngularVelocity() const override;
		void FLUFF_API SetPosition(const glm::vec3 & NewPosition);
		void FLUFF_API ApplyForce(const glm::vec3 & Force);
		void FLUFF_API * GetPointer() const override;
	};

	class PhysicsStaticActor : public PhysicsActor
	{
		struct PhysicsStaticActorImpl;
		std::unique_ptr<PhysicsStaticActorImpl> Impl_;
	public:
		PhysicsStaticActor(void * pSDK, glm::vec3 Position, glm::quat Rotation, GeometryType Geometry, GeometryDesc * GeomDesc, PhysicsMaterialDescriptor Material);
		glm::vec3 FLUFF_API GetPosition() const override;
		glm::vec3 FLUFF_API GetRotation() const override;
		glm::vec3 FLUFF_API GetLinearVelocity() const { return glm::vec3(0.0f); }
		glm::vec3 FLUFF_API GetAngularVelocity() const { return glm::vec3(0.0f); }
		void FLUFF_API * GetPointer() const override;
	};

} }