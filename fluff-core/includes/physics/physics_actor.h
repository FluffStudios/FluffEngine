#pragma once

#include <Physx/PxRigidActor.h>
#include <Physx/PxRigidStatic.h>
#include <Physx/PxPhysics.h>
#include <Physx/PxRigidDynamic.h>

#include <Physx/PxScene.h>

#include <glm/vec3.hpp>

namespace luminos { namespace physics {

	enum ActorType
	{
		STATIC,
		DYNAMIC
	};

	class PhysicsActor
	{
	protected:
		physx::PxTransform Pose_;
	public:
		virtual glm::vec3 GetPosition() const = 0;
		virtual glm::vec3 GetRotation() const = 0;
		virtual glm::vec3 GetLinearVelocity() const = 0;
		virtual glm::vec3 GetAngularVelocity() const = 0;

		virtual void AddToScene(physx::PxScene * Scene) = 0;
		virtual void RemoveFromScene(physx::PxScene * Scene) = 0;
	};

	class PhysicsDynamicActor : public PhysicsActor
	{
		physx::PxRigidDynamic * pActor;
	public:
		PhysicsDynamicActor(physx::PxPhysics * pSDK, physx::PxTransform & Transform, physx::PxGeometry & Geometry, physx::PxMaterial & Material, float Density);
		glm::vec3 GetPosition() const override;
		glm::vec3 GetRotation() const override;
		glm::vec3 GetLinearVelocity() const override;
		glm::vec3 GetAngularVelocity() const override;
		void SetPosition(const glm::vec3 & NewPosition);
		physx::PxRigidDynamic * GetActor() const { return pActor; }
		void AddToScene(physx::PxScene * Scene) override;
		void RemoveFromScene(physx::PxScene * Scene) override;
		void ApplyForce(const glm::vec3 & Force);
	};

	class PhysicsStaticActor : public PhysicsActor
	{
		physx::PxRigidStatic * pActor;
	public:
		PhysicsStaticActor(physx::PxPhysics * pSDK, physx::PxTransform & Transform, physx::PxGeometry & Geometry, physx::PxMaterial & Material);
		glm::vec3 GetPosition() const override;
		glm::vec3 GetRotation() const override;
		glm::vec3 GetLinearVelocity() const { return glm::vec3(0.0f); }
		glm::vec3 GetAngularVelocity() const { return glm::vec3(0.0f); }
		physx::PxRigidStatic * GetActor() const { return pActor; }
		void AddToScene(physx::PxScene * Scene) override;
		void RemoveFromScene(physx::PxScene * Scene) override;
	};

} }